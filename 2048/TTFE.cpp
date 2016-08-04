//
// Created by mcn on 7/27/16.
//

#include "TTFE.h"
#include "../exceptions/noCellLeft.h"
#include "../exceptions/endOfGrid.h"

#include <vector>
#include <random>
#include <iomanip>

using namespace std;

extern default_random_engine random_engine;

TTFE::TTFE() : score(0)
{
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[0].size(); j++)
        {
            grid[i][j] = Cell(tuple<int, int>(i, j));
        }
    }

    generateNewCell();
    generateNewCell();
}

tuple<int, int> TTFE::generateNewCell()
{
    vector<Cell*> possible = getEmptyCells();

    if (possible.size() == 0)
        throw noCellLeft();

    auto picker = uniform_int_distribution<int>(0, possible.size()-1);
    Cell &selected = *possible[picker(random_engine)];

    vector<int> numbers;
    numbers.push_back(2);
    numbers.push_back(2);
    numbers.push_back(4);

    auto picker2 = uniform_int_distribution<int>(0, 2);
    selected.number = numbers[picker2(random_engine)];

    return selected.position;
}

std::vector<Cell*> TTFE::getEmptyCells()
{
    vector<Cell*> empty;

    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[0].size(); j++)
        {
            if (grid[i][j].number == 0)
                empty.push_back(&grid[i][j]);
        }
    }

    return empty;
}

const array<array<Cell, 4>, 4> &TTFE::getGrid() const
{
    return grid;
}

int TTFE::getScore() const
{
    return score;
}

bool TTFE::makeAction(Direction direction)
{
    if (!checkMove(direction))
        return false;

    move(direction);
    merge(direction);
    move(direction);
    generateNewCell();
    return true;
}

bool TTFE::checkMove()
{
    return (checkMove(UP) || checkMove(DOWN) || checkMove(LEFT) || checkMove(RIGHT));
}

bool TTFE::checkMove(Direction direction)
{
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[0].size(); j++)
        {
            if (checkCellMove(grid[i][j].position, direction))
                return true;
        }
    }

    return false;
}

bool TTFE::checkCellMove(std::tuple<int, int> position, Direction direction)
{
    try
    {
        Cell &currCell = getCell(position);
        Cell &nextCell = getNextCell(position, direction);

        return currCell.number != 0 && (nextCell.number == 0 || nextCell.number == currCell.number);
    }
    catch (endOfGrid e)
    {
        return false;
    }
}

Cell &TTFE::getCell(std::tuple<int, int> position)
{
    return grid[get<0>(position)][get<1>(position)];
}

Cell &TTFE::getNextCell(std::tuple<int, int> position, Direction direction)
{
    int y = get<0>(position);
    int x = get<1>(position);
    if (direction == UP)
    {
        if (y == 0)
            throw endOfGrid();

        return getCell(make_tuple(y-1, x));
    }
    else if (direction == DOWN)
    {
        if (y == 3)
            throw endOfGrid();

        return getCell(make_tuple(y+1, x));
    }
    else if (direction == LEFT)
    {
        if (x == 0)
            throw endOfGrid();

        return getCell(make_tuple(y, x-1));
    }
    else
    {
        if (x == 3)
            throw endOfGrid();

        return getCell(make_tuple(y, x+1));
    }
}

void TTFE::move(Direction direction)
{
    if (direction == UP)
    {
        for (int i = 0; i < grid.size(); i++)
        {
            for (int j = 0; j < grid[0].size(); j++)
            {
                moveCell(grid[i][j].position, direction);
            }
        }
    }
    else if (direction == DOWN)
    {
        for (int i = grid.size()-1; i >=0; i--)
        {
            for (int j = 0; j < grid[0].size(); j++)
            {
                moveCell(grid[i][j].position, direction);
            }
        }
    }
    else if (direction == RIGHT)
    {
        for (int i = 0; i < grid.size(); i++)
        {
            for (int j = grid[0].size()-1; j >= 0; j--)
            {
                moveCell(grid[i][j].position, direction);
            }
        }
    }
    else
    {
        for (int i = 0; i < grid.size(); i++)
        {
            for (int j = 0; j < grid[0].size(); j++)
            {
                moveCell(grid[i][j].position, direction);
            }
        }
    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "InfiniteRecursion"
void TTFE::moveCell(std::tuple<int, int> position, Direction direction)
{
    try
    {
        Cell &currCell = getCell(position);
        Cell &nextCell = getNextCell(position, direction);

        if (nextCell.number != 0)
            return;

        nextCell.number = currCell.number;
        currCell.number = 0;

        moveCell(nextCell.position, direction);
    }
    catch (endOfGrid e)
    {
        return;
    }
}
#pragma clang diagnostic pop

void TTFE::merge(Direction direction)
{
    if (direction == UP)
    {
        for (int i = 0; i < grid.size(); i++)
        {
            for (int j = 0; j < grid[0].size(); j++)
            {
                mergeCell(grid[i][j].position, direction);
            }
        }
    }
    else if (direction == DOWN)
    {
        for (int i = grid.size()-1; i >=0; i--)
        {
            for (int j = 0; j < grid[0].size(); j++)
            {
                mergeCell(grid[i][j].position, direction);
            }
        }
    }
    else if (direction == RIGHT)
    {
        for (int i = 0; i < grid.size(); i++)
        {
            for (int j = grid[0].size()-1; j >= 0; j--)
            {
                mergeCell(grid[i][j].position, direction);
            }
        }
    }
    else
    {
        for (int i = 0; i < grid.size(); i++)
        {
            for (int j = 0; j < grid[0].size(); j++)
            {
                mergeCell(grid[i][j].position, direction);
            }
        }
    }
}

void TTFE::mergeCell(std::tuple<int, int> position, Direction direction)
{
    try
    {
        Cell &currCell = getCell(position);
        Cell &nextCell = getNextCell(position, direction);

        if (nextCell.number != currCell.number)
            return;

        nextCell.number *= 2;
        currCell.number = 0;

        score += nextCell.number;
    }
    catch (endOfGrid e)
    {
        return;
    }
}

int TTFE::mainTTFE()
{
    string commande = "";
    TTFE game = TTFE();
    bool refresh = true;

    cout << "Tapez \"aide\" pour obtenir une liste des commandes";

    while (!(commande == "exit"))
    {
        array<array<Cell, 4>, 4> theGrid = game.getGrid();

        if (refresh)
        {
            cout << endl;
            cout << "Etat du tableau actuellement :" << endl;

            cout << "+-----+-----+-----+-----+" << endl;
            cout << "|" << setw(5) << theGrid[0][0].number
                 << "|" << setw(5) << theGrid[0][1].number
                 << "|" << setw(5) << theGrid[0][2].number
                 << "|" << setw(5) << theGrid[0][3].number << "|" << endl;
            cout << "+-----+-----+-----+-----+" << endl;
            cout << "|" << setw(5) << theGrid[1][0].number
                 << "|" << setw(5) << theGrid[1][1].number
                 << "|" << setw(5) << theGrid[1][2].number
                 << "|" << setw(5) << theGrid[1][3].number << "|" << endl;
            cout << "+-----+-----+-----+-----+" << endl;
            cout << "|" << setw(5) << theGrid[2][0].number
                 << "|" << setw(5) << theGrid[2][1].number
                 << "|" << setw(5) << theGrid[2][2].number
                 << "|" << setw(5) << theGrid[2][3].number << "|" << endl;
            cout << "+-----+-----+-----+-----+" << endl;
            cout << "|" << setw(5) << theGrid[3][0].number
                 << "|" << setw(5) << theGrid[3][1].number
                 << "|" << setw(5) << theGrid[3][2].number
                 << "|" << setw(5) << theGrid[3][3].number << "|" << endl;
            cout << "+-----+-----+-----+-----+" << endl << endl;

            refresh = false;
        }

        cin >> commande;

        if ((commande == "droite"))
        {
            refresh = game.makeAction(RIGHT);
        }
        else if ((commande == "gauche"))
        {
            refresh = game.makeAction(LEFT);
        }
        else if ((commande == "haut"))
        {
            refresh = game.makeAction(UP);
        }
        else if ((commande == "bas"))
        {
            refresh = game.makeAction(DOWN);
        }
        else if ((commande == "isOver"))
        {
            if (game.checkMove())
                cout << "On peut encore bouger !" << endl;
            else
                cout << "Désolé ! Partie terminée" << endl;
        }
        else if ((commande == "score"))
        {
            cout << "Le score actuel est de " << game.getScore() << endl;
        }
        else if ((commande == "aide"))
        {
            cout << "Quelle commande voulez-vous executer ?" << endl;
            cout << "droite, gauche, haut, bas : déplace le tableau dans la direction demandée" << endl;
            cout << "score : Affiche le score" << endl;
            cout << "isOver : vérifie si la partie est finie ou non" << endl;
            cout << "afficher : affiche le tableau" << endl;
            cout << "exit : quitte le logiciel" << endl << endl;
        }
        else if ((commande == "afficher"))
        {
            refresh = true;
        }
        else if (!(commande == "exit"))
        {
            cout <<"Commande non reconnue" << endl;
        }
    }
}
