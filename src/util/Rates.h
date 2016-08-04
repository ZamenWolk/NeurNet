//
// Created by mcn on 7/26/16.
//

#ifndef NEURNET_RATES_H
#define NEURNET_RATES_H


#include <cereal/cereal.hpp>

struct Rates
{
public:

    Rates();

    double connectionMutation;
    double linkMutation;
    double nodeMutation;
    double biasMutation;
    double stepSize;
    double disableMutation;
    double enableMutation;

    void mutateRates();

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive(CEREAL_NVP(connectionMutation),
                CEREAL_NVP(linkMutation),
                CEREAL_NVP(nodeMutation),
                CEREAL_NVP(biasMutation),
                CEREAL_NVP(stepSize),
                CEREAL_NVP(disableMutation),
                CEREAL_NVP(enableMutation));
    }

    bool operator==(const Rates &rhs) const;

    bool operator!=(const Rates &rhs) const;

private:
    constexpr static double _connectionMutation = 0.25;
    constexpr static double _linkMutation = 2.0;
    constexpr static double _nodeMutation = 0.5;
    constexpr static double _biasMutation = 0.4;
    constexpr static double _stepSize = 0.1;
    constexpr static double _disableMutation = 0.4;
    constexpr static double _enableMutation = 0.2;

    void mutateRate(double &rate);
};


#endif //NEURNET_RATES_H
