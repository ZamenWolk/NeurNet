//
// Created by mcn on 7/27/16.
//

#ifndef NEURNET_TTFE_H
#define NEURNET_TTFE_H

#include <array>
#include <cereal/types/vector.hpp>
#include "Cell.h"
#include "Direction.h"

class TTFE
{
public:

    TTFE();

    const std::array<std::array<Cell, 4>, 4> &getGrid() const;
    int getScore() const;

    bool makeAction(Direction direction);

    bool checkMove();
    bool checkMove(Direction direction);

    static int mainTTFE();

private:

    std::array<std::array<Cell, 4>, 4> grid;
    int score;

    std::tuple<int, int> generateNewCell();

    std::vector<Cell*> getEmptyCells();

    bool checkCellMove(std::tuple<int, int> position, Direction direction);

    Cell &getCell(std::tuple<int, int> position);

    Cell &getNextCell(std::tuple<int, int> position, Direction direction);

    void move(Direction direction);

    void merge(Direction direction);

    void moveCell(std::tuple<int, int> position, Direction direction);

    void mergeCell(std::tuple<int, int> position, Direction direction);
};


#endif //NEURNET_TTFE_H
