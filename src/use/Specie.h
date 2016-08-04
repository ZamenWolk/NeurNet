//
// Created by Martin on 01/08/2016.
//

#ifndef NEURNET_SPECIE_H
#define NEURNET_SPECIE_H

#include <functional>
#include <deque>
#include "../storage/Genome.h"

class Pool;

//TODO finish that
class Specie
{
public:

    Specie(const double deltaDisjoint, const double deltaWeight, const double deltaTreshold);

    double currentTopFitness();

    double getTopFitness() const;

    void setTopFitness(double topFitness);

    int getStaleness() const;

    void resetStaleness();

    double averageFitness();

    bool sameSpecie(const Genome &genome) const;

    void addToSpecie(Genome &genome);

    std::deque<Genome> *getGenomes();

    void increaseStaleness();

    void cull(bool toOne);

    Genome breedChild(double crossoverChance, double perturbChance);

    bool operator==(const Specie &rhs) const;

    bool operator!=(const Specie &rhs) const;

private:

    std::deque<Genome> genomes;
    double topFitness;
    int staleness;
    double deltaDisjoint;
    double deltaWeight;
    double deltaTreshold;
};


#endif //NEURNET_SPECIE_H
