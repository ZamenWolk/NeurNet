//
// Created by Martin on 01/08/2016.
//

#include <algorithm>
#include <random>
#include "Specie.h"
#include "Pool.h"

using namespace std;

extern default_random_engine random_engine;

double Specie::currentTopFitness()
{
    double top{genomes[0].getFitness()};

    for (auto it = genomes.begin(); it != genomes.end(); it++) {
        if (it->getFitness() > top)
            top = it->getFitness();
    }

    return top;
}

double Specie::getTopFitness() const {
    return topFitness;
}

void Specie::setTopFitness(double topFitness) {
    Specie::topFitness = topFitness;
}

int Specie::getStaleness() const {
    return staleness;
}

void Specie::resetStaleness() {
    staleness = 0;
}

double Specie::averageFitness() {
    double add{0};

    for (auto it = genomes.begin(); it != genomes.end(); it++)
    {
        add += it->getFitness();
    }

    return add/genomes.size();
}

bool Specie::sameSpecie(const Genome &genome) const {
    double disjoint = deltaDisjoint * Genome::disjointFactor(genome, genomes[0]);
    double weight = deltaWeight * Genome::weightFactor(genome, genomes[0]);

    return disjoint + weight < deltaTreshold;
}

deque<Genome> *Specie::getGenomes() {
    return &genomes;
}

void Specie::increaseStaleness() {
    staleness++;
}

void Specie::addToSpecie(Genome &genome) {
    genomes.push_back(genome);
}

Specie::Specie(const double deltaDisjoint,
               const double deltaWeight,
               const double deltaTreshold) : deltaDisjoint(deltaDisjoint),
                                             deltaWeight(deltaWeight),
                                             deltaTreshold(deltaTreshold),
                                             genomes{},
                                             topFitness(0),
                                             staleness(0)
{}

void Specie::cull(bool toOne) {
    sort(genomes.begin(), genomes.end(), [](const Genome &g1, const Genome &g2) {return g1.getFitness() > g2.getFitness();});

    int remainGoal = toOne ? 1 : (genomes.size()+1)/2;

    while (genomes.size() > remainGoal)
    {
        genomes.pop_back();
    }
}

Genome Specie::breedChild(double crossoverChance, double perturbChance) {
    uniform_real_distribution<double> realDistribution(0, 1);
    uniform_int_distribution<int> picker(0, genomes.size()-1);

    if (realDistribution(random_engine) < crossoverChance)
    {
        const Genome &g1 = genomes.at(picker(random_engine));
        const Genome &g2 = genomes.at(picker(random_engine));
        Genome child{g1, g2};
        child.mutate(perturbChance);
        return child;
    }
    else
    {
        const Genome &g = genomes[picker(random_engine)];
        Genome child{g};
        child.mutate(perturbChance);
        return child;
    }
}

bool Specie::operator==(const Specie &rhs) const {
    return genomes == rhs.genomes &&
           staleness == rhs.staleness &&
           deltaDisjoint == rhs.deltaDisjoint &&
           deltaWeight == rhs.deltaWeight &&
           deltaTreshold == rhs.deltaTreshold;
}

bool Specie::operator!=(const Specie &rhs) const {
    return !(rhs == *this);
}
