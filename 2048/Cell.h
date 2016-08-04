//
// Created by mcn on 7/27/16.
//

#ifndef NEURNET_CELL_H
#define NEURNET_CELL_H

#include <tuple>

struct Cell
{
public:

    Cell(const std::tuple<int, int> &position);

    std::tuple<int, int> position;
    int number;

private:

public:
    Cell();
};


#endif //NEURNET_CELL_H
