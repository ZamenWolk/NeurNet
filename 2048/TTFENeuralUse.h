//
// Created by Martin on 27/07/2016.
//

#ifndef NEURNET_TTFENEURALUSE_H
#define NEURNET_TTFENEURALUSE_H

#include <vector>
#include "TTFE.h"

struct TTFENeuralUse
{
    static int neuralUse();

    static std::vector<double> generateInputs(std::vector<int> numbers);
};


#endif //NEURNET_TTFENEURALUSE_H
