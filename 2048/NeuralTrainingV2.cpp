//
// Created by mcn on 8/2/16.
//

#include "NeuralTrainingV2.h"
#include "../src/use/Pool.h"

int NeuralTrainingV2::main() {

    Pool pool{500, 2, 0.4, 1, 15, 0.75, 0.95, 16, 4};

    while (1) {
        pool.evaluate(function);

        cout << "Sum of averages : " << pool.sumAverageFitness() << endl;
        cout << "Global top : " << pool.topFitness() << endl;

        pool.newGeneration();

        cout << endl << "New generation" << endl;
    }

    return 0;
}

std::vector<double> NeuralTrainingV2::generateInputs(TTFE &ttfe) {
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

NeuralTrainingV2::NeuralTrainingV2() {
    function = [this](Genome& genome)
    {
        vector<double> scores;
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
                scores.push_back(0);
        }

        return accumulate(scores.begin(), scores.end(), 0.0)/scores.size();
    };
}
