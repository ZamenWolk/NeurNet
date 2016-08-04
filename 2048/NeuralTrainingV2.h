//
// Created by mcn on 8/2/16.
//

#ifndef NEURNET_NEURALTRAININGV2_H
#define NEURNET_NEURALTRAININGV2_H

#include <functional>
#include <algorithm>
#include <numeric>
#include "../src/storage/Genome.h"
#include "TTFE.h"

using namespace std;

class NeuralTrainingV2 {
public:

    NeuralTrainingV2();

    int main();

    std::vector<double> generateInputs(TTFE &ttfe);

    std::function<double(Genome&)> function;
};


#endif //NEURNET_NEURALTRAININGV2_H
