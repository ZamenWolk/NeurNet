//
// Created by mcn on 7/26/16.
//

#include <vector>
#include <random>
#include <algorithm>
#include "Genome.h"
#include "../util/util.h"

using namespace std;

extern default_random_engine random_engine;

const vector<Gene> Genome::getGenes() const
{
    return genes;
}

int Genome::getMaxNeurons() const
{
    return neuronNumber;
}

Genome::Genome(int inputs,  int outputs) : genes{}, neuronNumber{inputs+1}, rates{}, inputs{inputs}, outputs{outputs}, fitness{0}
{

}

void Genome::mutate(double perturbChance)
{
    auto picker = uniform_real_distribution<double>(0, 1);
    double p;

    rates.mutateRates();

    if (picker(random_engine) < rates.connectionMutation)
        pointMutate(perturbChance);

    p = rates.linkMutation;
    while (p > 0)
    {
        if (picker(random_engine) < p)
            linkMutate(false);
        p--;
    }

    p = rates.biasMutation;
    while (p > 0)
    {
        if (picker(random_engine) < p)
            linkMutate(true);
        p--;
    }

    p = rates.nodeMutation;
    while (p > 0)
    {
        if (picker(random_engine) < p)
            nodeMutate();
        p--;
    }

    p = rates.enableMutation;
    while (p > 0)
    {
        if (picker(random_engine) < p)
            enableDisableMutate(true);
        p--;
    }

    p = rates.disableMutation;
    while (p > 0)
    {
        if (picker(random_engine) < p)
            enableDisableMutate(false);
        p--;
    }
}

void Genome::pointMutate(double perturbChance)
{
    double step = rates.stepSize;

    for (auto it = genes.begin(); it != genes.end(); it++)
    {
        it->pointMutate(step, perturbChance);
    }
}

void Genome::linkMutate(bool forceBias)
{
    int neuronFrom = forceBias ? inputs : randomNeuron(false, true);
    int neuronTo = randomNeuron(true, false);

    if (containsLink(neuronFrom, neuronTo) || neuronFrom == neuronTo || wouldCircle(neuronFrom, neuronTo))
        return;

    auto picker = uniform_real_distribution<double>(-2, 2);
    Gene newGene(neuronFrom, neuronTo, picker(random_engine), newInnovation());

    genes.push_back(newGene);
}

int Genome::randomNeuron(bool nonInput, bool nonOutput)
{
    std::vector<int> neurons{};

    if (!nonInput)
        for (int i = 0; i < inputs; i++)
            neurons.push_back(i);

    if (!nonOutput)
        for (int i = 0; i < outputs; i++)
            neurons.push_back(MAX_NEURONS+i);

    for (auto it = genes.begin(); it != genes.end(); it++)
    {
        auto found = find(neurons.begin(), neurons.end(), it->getFrom());
        if (found == neurons.end())
            if (!nonInput || it->getFrom() > inputs)
                if (!nonOutput || it->getFrom() < MAX_NEURONS)
                    neurons.push_back(it->getFrom());
        found = find(neurons.begin(), neurons.end(), it->getTo());
        if (found == neurons.end())
            if (!nonInput || it->getTo() > inputs)
                if (!nonOutput || it->getTo() < MAX_NEURONS)
                    neurons.push_back(it->getTo());
    }

    auto picker = uniform_int_distribution<int>(0, neurons.size()-1);
    int nbr = picker(random_engine);
    int result = neurons[picker(random_engine)];
    return result;
}

bool Genome::containsLink(int from, int to)
{
    for (auto it = genes.begin(); it != genes.end(); it++)
    {
        if (it->getFrom() == from && it->getTo() == to)
            return true;
    }

    return false;
}

void Genome::nodeMutate()
{
    auto picker = uniform_int_distribution<int>(0, genes.size()-1);
    Gene &gene = genes[picker(random_engine)];
    Gene newGeneFrom(gene), newGeneTo(gene);
    int newNeuron = addNeuron();

    gene.disable();

    newGeneFrom.setTo(newNeuron);
    newGeneFrom.setInnovation(newInnovation());
    newGeneFrom.setWeight(1);
    newGeneFrom.enable();

    newGeneTo.setFrom(newNeuron);
    newGeneTo.setInnovation(newInnovation());
    newGeneTo.enable();

    genes.push_back(newGeneFrom);
    genes.push_back(newGeneTo);
}

int Genome::addNeuron()
{
    return neuronNumber++;
}

void Genome::enableDisableMutate(bool enable)
{
    vector<Gene*> candidates;

    for (auto it = genes.begin(); it != genes.end(); it++)
    {
        if (it->isEnabled() != enable)
            candidates.push_back(it.base());
    }

    if (candidates.size() == 0)
        return;

    auto picker = uniform_int_distribution<int>(0, candidates.size()-1);
    Gene *gene = candidates[picker(random_engine)];

    enable ? gene->enable() : gene->disable();
}

Genome Genome::basicGenome(int inputs, int outputs)
{
    Genome genome(inputs, outputs);
    genome.mutate();

    return genome;
}

int Genome::getInputs() const
{
    return inputs;
}

int Genome::getOutputs() const
{
    return outputs;
}

NeuralNetwork Genome::getNetwork()
{
    return NeuralNetwork(*this);
}

Genome::Genome(Genome a, Genome b) : Genome(a.inputs, a.outputs)
{
    fitness = 0;
    auto picker = uniform_int_distribution<int>(0, 1);
    if (a.getFitness() < b.getFitness())
    {
        Genome temp = a;
        a = b;
        b = temp;
    }

    if (a.getInputs() != b.getInputs() || a.getOutputs() != b.getOutputs())
        throw runtime_error("The genomes are not compatible !");

    this->inputs = a.getInputs();
    this->outputs = a.getOutputs();

    std::map<int, Gene*> innovationGenesB;
    auto genesB = b.getGenes();
    for (auto it = genesB.begin(); it != genesB.end(); it++)
    {
        innovationGenesB[it->getInnovation()] = it.base();
    }

    auto genesA = a.getGenes();
    for (auto it = genesA.begin(); it != genesA.end(); it++)
    {
        Gene *geneA = &*it;
        Gene *geneB = innovationGenesB[geneA->getInnovation()];

        if (geneB != NULL && picker(random_engine) && geneB->isEnabled())
            genes.push_back(*geneB);
        else
            genes.push_back(*geneA);
    }

    neuronNumber = max(a.getMaxNeurons(), b.getMaxNeurons());
    rates = a.rates;
}

double Genome::getFitness() const
{
    return fitness;
}

void Genome::setFitness(double fitness)
{
    Genome::fitness = fitness;
}

Genome::Genome(int inputs, int outputs, int neuronNumber, double fitness, const Rates &rates, const std::vector<Gene> &genes) :
        genes(genes), inputs(inputs), outputs(outputs), neuronNumber(neuronNumber), rates(rates), fitness(fitness)
{}

Genome::~Genome()
{

}

bool Genome::wouldCircle(int from, int to)
{
    vector<Gene> originsOfFrom;

    for (auto it = genes.begin(); it != genes.end(); it++)
    {
        if (it->getTo() == from)
            originsOfFrom.push_back(*it);
    }

    for (auto it = originsOfFrom.begin(); it != originsOfFrom.end(); it++)
    {
        if (it->getFrom() == to)
            return true;

        if (wouldCircle(it->getFrom(), to))
            return true;
    }

    return false;
}

double Genome::disjointFactor(const Genome &g1, const Genome &g2)
{
    map<int, bool> g1Map, g2Map;
    int disjointNumber{0};

    auto g2g = g2.getGenes();
    auto g1g = g1.getGenes();

    for (auto it = g1g.begin(); it != g1g.end(); it++)
    {
        g1Map[it->getInnovation()] = true;
    }

    for (auto it = g2g.begin(); it != g2g.end(); it++)
    {
        g2Map[it->getInnovation()] = true;
    }

    for (auto it = g1Map.begin(); it != g1Map.end(); it++)
    {
        if (g2Map.find(it->first) == g2Map.end())
            disjointNumber++;
    }

    for (auto it = g2Map.begin(); it != g2Map.end(); it++)
    {

        if (g1Map.find(it->first) == g1Map.end())
            disjointNumber++;
    }

    return (double)(disjointNumber) / max(g1.getGenes().size(), g2.getGenes().size());
}

double Genome::weightFactor(const Genome &g1, const Genome &g2)
{
    map<int, const Gene*> g2Genes;

    double sum{0};
    int coincident{0};

    auto g2g = g2.getGenes();
    auto g1g = g1.getGenes();

    for (auto it = g2g.begin(); it != g2g.end(); it++)
    {
        g2Genes[it->getInnovation()] = it.base();
    }

    for (auto it = g1g.begin(); it != g1g.end(); it++)
    {
        if (g2Genes.find(it->getInnovation()) != g2Genes.end())
        {
            sum += abs(it->getWeight() - g2Genes[it->getInnovation()]->getWeight());
            coincident++;
        }
    }

    return sum / coincident;
}

bool Genome::operator==(const Genome &rhs) const {
    return genes == rhs.genes &&
           inputs == rhs.inputs &&
           outputs == rhs.outputs &&
           neuronNumber == rhs.neuronNumber &&
           rates == rhs.rates;
}

bool Genome::operator!=(const Genome &rhs) const {
    return !(rhs == *this);
}

Genome::Genome(const Genome &g) : genes(g.genes), inputs(g.inputs) ,outputs(g.outputs),neuronNumber(g.neuronNumber),rates(g.rates),fitness()
{

}
