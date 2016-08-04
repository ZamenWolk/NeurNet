//
// Created by Martin on 27/07/2016.
//

#include <cereal/archives/json.hpp>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include "TTFENeuralUse.h"
#include "../src/storage/Genome.h"

using namespace std;

int TTFENeuralUse::neuralUse()
{
    Genome genome(16, 4);
    {
        ifstream file("use.json");
        cereal::JSONInputArchive archive(file);
        archive(genome);
    }

    NeuralNetwork network = genome.getNetwork();

    while(true)
    {
        vector<int> inputs{};
        int input;
        cout << endl << "Entrez vos inputs separes par un espace" << endl;

        while ((cin >> input) && input != -1)
            inputs.push_back(input);

        vector<double> outputs = network.evaluate(generateInputs(inputs));

        if (count(outputs.begin(), outputs.end(), *max_element(outputs.begin(), outputs.end())) > 1)
            cout << "Error in network" << endl;
        else
        {
            Direction direction = (Direction) ((int) (distance(outputs.begin(),
                                                               max_element(outputs.begin(), outputs.end()))));

            switch (direction)
            {
                case UP:
                    cout << "Go up" << endl;
                    break;
                case DOWN:
                    cout << "Go down" << endl;
                    break;
                case LEFT:
                    cout << "Go left" << endl;
                    break;
                case RIGHT:
                    cout << "Go right" << endl;
                    break;
            }
        }
    }

    return 0;
}

std::vector<double> TTFENeuralUse::generateInputs(std::vector<int> numbers)
{
    vector<double> result{};

    for (auto number : numbers)
    {
        int numerator = 0;

        while (number > 1)
        {
            number /= 2;
            numerator++;
        }

        result.push_back(numerator/15.0);
    }

    return result;
}
