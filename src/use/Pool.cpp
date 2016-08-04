//
// Created by Martin on 01/08/2016.
//

#include "Pool.h"
#include <algorithm>
#include <random>

using namespace std;

extern default_random_engine random_engine;

Pool::Pool(int population, double deltaDisjoint, double deltaWeight, double deltaTreshold, int iterationsBeforeStale,
           double crossoverChance, double perturbChance, int inputs, int outputs) : population(population),
                                                                                    deltaDisjoint(deltaDisjoint),
                                                                                    deltaWeight(deltaWeight),
                                                                                    deltaTreshold(deltaTreshold),
                                                                                    iterationsBeforeStale(iterationsBeforeStale),
                                                                                    species(),
                                                                                    crossoverChance(crossoverChance),
                                                                                    perturbChance(perturbChance)
{
    while (poolSize() < population)
    {
        addGenome(Genome::basicGenome(inputs, outputs));
    }
}

void Pool::evaluate(std::function<double(Genome &)> &evaluationFunction)
{
    for(auto it = species.begin(); it != species.end(); it++)
    {
        auto genomes = it->getGenomes();

        for (auto iter = genomes->begin(); iter != genomes->end(); iter++)
        {
            iter->setFitness(evaluationFunction(*iter));
        }
    }
}

void Pool::removeStaleSpecies()
{
    deque<Specie> speciesToDelete{};
    for (auto it = species.begin(); it != species.end(); it++)
    {
        if (it->currentTopFitness() > it->getTopFitness())
        {
            it->resetStaleness();
            it->setTopFitness(it->currentTopFitness());
        }
        else
            it->increaseStaleness();

        if (it->getStaleness() > iterationsBeforeStale && it->getTopFitness() < maxFitness())
            speciesToDelete.push_back(*it);
    }

    for (auto it = speciesToDelete.begin(); it != speciesToDelete.end(); it++) {
        removeSpecie(*it);
    }
}

double Pool::sumAverageFitness() {
    double add{0};

    for (auto it = species.begin(); it != species.end(); it++) {
        add += it->averageFitness();
    }

    return add;
}

void Pool::removeWeakSpecies() {

    double add{sumAverageFitness()};
    deque<Specie> speciesToDelete{};

    for (auto it = species.begin(); it != species.end(); it++)
    {
        if (it->averageFitness() / add * population < 1)
            speciesToDelete.push_back(*it);
    }

    for (auto it = speciesToDelete.begin(); it != speciesToDelete.end(); it++) {
        removeSpecie(*it);
    }
}

void Pool::removeSpecie(Specie &specie)
{
    species.erase(find(species.begin(), species.end(), specie));
}

void Pool::addGenome(Genome genome) {
    for (auto it = species.begin(); it != species.end(); it++)
    {
        if (it->sameSpecie(genome)) {
            it->addToSpecie(genome);
            return;
        }
    }

    addSpecie().addToSpecie(genome);
}

double Pool::maxFitness() {
    double max = species[0].getTopFitness();

    for (auto it = species.begin(); it != species.end(); it++)
    {
        if (it->getTopFitness() > max)
            max = it->getTopFitness();
    }

    return max;
}

Specie &Pool::addSpecie() {
    species.push_back(Specie(deltaDisjoint, deltaWeight, deltaTreshold));

    return species.back();
}

void Pool::newGeneration() {
    removeStaleSpecies();
    removeWeakSpecies();

    deque<Genome> children{};
    double sum = sumAverageFitness();
    uniform_int_distribution<int> picker(0, species.size()-1);

    for (auto it = species.begin(); it != species.end(); it++)
    {
        it->cull(false);
        int breed = it->averageFitness() / sum * population -1;

        for (auto i = 0; i < breed; i++)
            children.push_back(it->breedChild(crossoverChance, perturbChance));

        it->cull(true);
    }

    while (children.size() + poolSize() < population)
    {
        children.push_back(species[picker(random_engine)].breedChild(crossoverChance, perturbChance));
    }

    for (auto it = children.begin(); it != children.end(); it++) {
        addGenome(*it);
    }
}

int Pool::poolSize() {
    int size{0};

    for (auto it = species.begin(); it != species.end(); it++) {
      size += it->getGenomes()->size();
    }

    return size;
}

double Pool::topFitness() {
    double result = species[0].currentTopFitness();

    for (auto it = species.begin(); it != species.end(); it++) {
        if (it->currentTopFitness() > result)
            result = it->currentTopFitness();
    }

    return result;
}
