//
// Created by Martin on 01/08/2016.
//

#ifndef NEURNET_POOL_H
#define NEURNET_POOL_H

#include <functional>
#include <vector>
#include "../storage/Genome.h"
#include "Specie.h"

//TODO finish that
class Pool
{
public:

    Pool(int population, double deltaDisjoint, double deltaWeight, double deltaTreshold, int iterationsBeforeStale,
             double crossoverChance, double perturbChance, int inputs, int outputs);

    void evaluate(std::function<double(Genome&)> &evaluationFunction);

    double sumAverageFitness();

    void newGeneration();

    void addGenome(Genome genome);

    double topFitness();

private:

    void removeWeakSpecies();
    void removeStaleSpecies();

    void removeSpecie(Specie &specie);

    Specie &addSpecie();

    std::deque<Specie> species;
    const int population;
    const double deltaDisjoint;
    const double deltaWeight;
    const double deltaTreshold;
    const int iterationsBeforeStale;
    const double crossoverChance;
    const double perturbChance;

    double maxFitness();

    int poolSize();
};


#endif //NEURNET_POOL_H
