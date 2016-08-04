//
// Created by mcn on 7/26/16.
//

#ifndef NEURNET_GENOME_H
#define NEURNET_GENOME_H

#include <vector>
#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include "Gene.h"
#include "../util/Rates.h"
#include "../network/NeuralNetwork.h"

class Genome
{
public:

    Genome(int inputs, int outputs);
    Genome(Genome a, Genome b);
    Genome(const Genome &g);
    ~Genome();

    const std::vector<Gene> getGenes() const;

    int getMaxNeurons() const;

    void mutate(double perturbChance = 1);

    static Genome basicGenome(int inputs, int outputs);

    int getInputs() const;

    int getOutputs() const;

    NeuralNetwork getNetwork();

    double getFitness() const;

    void setFitness(double fitness);

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive(CEREAL_NVP(inputs), CEREAL_NVP(outputs), CEREAL_NVP(neuronNumber), CEREAL_NVP(fitness), CEREAL_NVP(rates), CEREAL_NVP(genes));
    }

    template <class Archive>
    static void load_and_construct(Archive & ar, cereal::construct<Genome> &construct)
    {
        int inputs, outputs, neuronNumber;
        std::vector<Gene> genes;
        Rates rates;
        double fitness;

        ar(inputs, outputs, neuronNumber, fitness, rates, CEREAL_NVP(genes));
        construct(inputs, outputs, neuronNumber, fitness, rates, genes);
    }

    static double disjointFactor(const Genome &g1, const Genome &g2);

    static double weightFactor(const Genome &g1, const Genome &g2);

    bool operator==(const Genome &rhs) const;

    bool operator!=(const Genome &rhs) const;

private:

    Genome(int inputs, int outputs, int neuronNumber, double fitness, const Rates &rates, const std::vector<Gene> &genes);

    std::vector<Gene> genes;
    int inputs;
    int outputs;
    int neuronNumber;
    Rates rates;
    double fitness;

    void pointMutate(double perturbChance);
    void linkMutate(bool forceBias);
    bool containsLink(int neuron1, int neuron2);
    int addNeuron();

    int randomNeuron(bool nonInput, bool nonOutput);

    void nodeMutate();

    void enableDisableMutate(bool enable);

    bool wouldCircle(int from, int to);
};


#endif //NEURNET_GENOME_H
