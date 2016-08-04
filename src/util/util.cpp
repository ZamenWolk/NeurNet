//
// Created by mcn on 7/27/16.
//

#include <random>
#include <chrono>

#include "util.h"

std::default_random_engine random_engine(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

int newInnovation()
{
    static int innovation = 1;

    return innovation++;
}
