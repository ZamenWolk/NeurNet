//
// Created by mcn on 7/27/16.
//

#ifndef NEURNET_TTFENEURALTRAINING_H
#define NEURNET_TTFENEURALTRAINING_H

#include <vector>
#include "TTFE.h"

struct TTFENeuralTraining
{
    static int neuralTraining();

    static std::vector<double> generateInputs(TTFE &ttfe);
};


#endif //NEURNET_TTFENEURALTRAINING_H
