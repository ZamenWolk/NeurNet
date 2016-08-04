//
// Created by mcn on 7/27/16.
//

#include "Cell.h"

Cell::Cell(const std::tuple<int, int> &position) : position(position), number(0)
{}

Cell::Cell() : position(std::make_tuple(-1, -1)), number(0)
{}
