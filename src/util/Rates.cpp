//
// Created by mcn on 7/26/16.
//

#include <random>
#include "Rates.h"

using namespace std;

Rates::Rates() : connectionMutation(_connectionMutation),
                 linkMutation(_linkMutation),
                 nodeMutation(_nodeMutation),
                 biasMutation(_biasMutation),
                 stepSize(_stepSize),
                 disableMutation(_disableMutation),
                 enableMutation(_enableMutation)
{

}

void Rates::mutateRates()
{
    mutateRate(connectionMutation);
    mutateRate(linkMutation);
    mutateRate(nodeMutation);
    mutateRate(biasMutation);
    mutateRate(stepSize);
    mutateRate(disableMutation);
    mutateRate(enableMutation);
}

void Rates::mutateRate(double &rate)
{
    default_random_engine generator;
    uniform_int_distribution<int> distribution(0, 1);

    if (distribution(generator))
        rate *= 0.95;
    else
        rate *= 1.05263;
}

bool Rates::operator==(const Rates &rhs) const {
    return connectionMutation == rhs.connectionMutation &&
           linkMutation == rhs.linkMutation &&
           nodeMutation == rhs.nodeMutation &&
           biasMutation == rhs.biasMutation &&
           stepSize == rhs.stepSize &&
           disableMutation == rhs.disableMutation &&
           enableMutation == rhs.enableMutation;
}

bool Rates::operator!=(const Rates &rhs) const {
    return !(rhs == *this);
}
