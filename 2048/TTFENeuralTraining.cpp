//
// Created by mcn on 7/27/16.
//

#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <cereal/archives/json.hpp>
#include <fstream>
#include <ctime>
#include "TTFENeuralTraining.h"
#include "../src/storage/Genome.h"
#include "TTFE.h"

using namespace std;

extern default_random_engine random_engine;

int TTFENeuralTraining::neuralTraining()
{
    bool stop = false;
    int iterations = 0;
    vector<Genome> pool;

    for (int i = 0; i < 48; i++)
        pool.push_back(Genome::basicGenome(16, 4));

    time_t beginTime = time(NULL);

    while(beginTime + 3600*17 > time(NULL))
    {
        iterations++;
        for (auto it = pool.begin(); it != pool.end(); it++)
        {
            vector<double> scores;
            Genome &genome = *it;
            for (int i = 0; i < 25; i++)
            {
                NeuralNetwork network = genome.getNetwork();
                TTFE ttfe{};
                bool lastSucceded = true, nonFunctional = false;

                while (lastSucceded && ttfe.checkMove())
                {
                    vector<double> outputs = network.evaluate(generateInputs(ttfe));

                    if (count(outputs.begin(), outputs.end(), *max_element(outputs.begin(), outputs.end())) > 1)
                        nonFunctional = true;

                    Direction direction = (Direction)((int)(distance(outputs.begin(), max_element(outputs.begin(), outputs.end()))));

                    lastSucceded = ttfe.makeAction(direction);
                }

                if (!nonFunctional)
                    scores.push_back(ttfe.getScore());
                else
                    scores.push_back(-1);
            }

            genome.setFitness(accumulate(scores.begin(), scores.end(), 0.0)/scores.size());
        }

        double averageFitness = 0;
        for (auto it = pool.begin(); it != pool.end(); it++)
        {
            averageFitness += it->getFitness();
        }
        averageFitness /= pool.size();
        cout << "Iteration " << iterations << " : " << averageFitness << endl;

        sort(pool.begin(), pool.end(), [] (Genome &g1, Genome &g2) {return g1.getFitness() > g2.getFitness();});

        cout << "Median fitness : " << pool.at(pool.size()/2-1).getFitness() << endl;

        while (pool.size() > 24)
        {
            pool.pop_back();
        }

        for (auto it = pool.begin(); it != pool.end();)
        {
            if (it->getFitness() < averageFitness)
                pool.erase(it);
            else
                it++;
        }

        {
            cout << "Best fit : " << pool[0].getFitness() << endl;
            stringstream stream;
            stream << "gen" << iterations << ".json";
            ofstream file(stream.str(), ios::trunc);
            cereal::JSONOutputArchive archive(file);
            archive(pool[0]);
            cout << endl << endl;
            file.close();
        }

        vector<Genome> newPool{pool};
        while(newPool.size() < 48)
        {
            auto picker = uniform_int_distribution<int>(0, pool.size()-1);
            Genome g1 = pool[picker(random_engine)];
            Genome g2 = pool[picker(random_engine)];
            Genome newGenome = Genome(g1, g2);
            newGenome.mutate();

            newPool.push_back(newGenome);
        }

        pool = newPool;
    }

    return 0;
}

std::vector<double> TTFENeuralTraining::generateInputs(TTFE &ttfe)
{
    auto grid = ttfe.getGrid();

    vector<double> inputs{};

    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[0].size(); j++)
        {
            int input = 0, gridNumber = grid[i][j].number;

            while (gridNumber > 1)
            {
                gridNumber /= 2;
                input++;
            }

            inputs.push_back(input/15.0);
        }
    }

    return inputs;
}
