//
// Created by mcn on 7/26/16.
//

#include <random>
#include "Gene.h"
#include "../util/util.h"

using namespace std;

extern default_random_engine random_engine;

int Gene::getTo() const
{
    return to;
}

int Gene::getFrom() const
{
    return from;
}

double Gene::getWeight() const
{
    return weight;
}

bool Gene::isEnabled() const
{
    return enabled;
}

int Gene::getInnovation() const
{
    return innovation;
}

void Gene::pointMutate(double stepSize, double perturb)
{
    auto weightRandom = uniform_real_distribution<double>(getWeight()-stepSize, getWeight()+stepSize);
    auto totalRandom = uniform_real_distribution<double>(-2, 2);
    auto picker = uniform_real_distribution<double>(0, 1);

    if (picker(random_engine) < perturb)
        weight = weightRandom(random_engine);
    else
        weight = totalRandom(random_engine);
}

Gene::Gene(int from, int to, double weight, int innovation) :
        from(from), to(to), weight(weight), enabled(true), innovation(innovation)
{}

void Gene::enable()
{
    enabled = true;
}

void Gene::disable()
{
    enabled = false;
}

void Gene::setFrom(int from)
{
    Gene::from = from;
}

void Gene::setTo(int to)
{
    Gene::to = to;
}

void Gene::setWeight(double weight)
{
    Gene::weight = weight;
}

void Gene::setInnovation(int innovation)
{
    Gene::innovation = innovation;
}

Gene::Gene(int from, int to, double weight, bool enabled, int innovation) :
        from(from), to(to), weight(weight), enabled(enabled), innovation(innovation)
{}

Gene::Gene() : from(-1), to (-1), weight(0), enabled(true), innovation(-1)
{}

bool Gene::operator==(const Gene &rhs) const {
    return from == rhs.from &&
           to == rhs.to &&
           weight == rhs.weight &&
           enabled == rhs.enabled &&
           innovation == rhs.innovation;
}

bool Gene::operator!=(const Gene &rhs) const {
    return !(rhs == *this);
}
