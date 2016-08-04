//
// Created by mcn on 7/26/16.
//

#ifndef NEURNET_GENE_H
#define NEURNET_GENE_H


#include <memory>
#include <cereal/access.hpp>
#include <cereal/cereal.hpp>

class Gene
{
public:

    Gene(int from, int to, double weight, int innovation);

    Gene(const Gene &gene) = default;

    Gene();

    int getTo() const;

    int getFrom() const;

    double getWeight() const;

    bool isEnabled() const;

    int getInnovation() const;

    void pointMutate(double stepSize, double perturb);

    void enable();
    void disable();

    void setFrom(int from);

    void setTo(int to);

    void setWeight(double weight);

    void setInnovation(int innovation);

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive(CEREAL_NVP(from), CEREAL_NVP(to), CEREAL_NVP(weight), CEREAL_NVP(enabled), CEREAL_NVP(innovation));
    }

    template <class Archive>
    static void load_and_construct(Archive & ar, cereal::construct<Gene> &construct)
    {
        int from, to, innovation;
        double weight;
        bool enabled;

        ar(from, to, weight, enabled, innovation);
        construct(from, to, weight, enabled, innovation);
    }

    bool operator==(const Gene &rhs) const;

    bool operator!=(const Gene &rhs) const;

private:

public:
    Gene(int from, int to, double weight, bool enabled, int innovation);

private:

    int from;
    int to;
    double weight;
    bool enabled;
    int innovation;
};


#endif //NEURNET_GENE_H
