//
// Created by mcn on 7/26/16.
//

#include <algorithm>
#include "NeuralNetwork.h"
#include "../util/util.h"
#include "../storage/Genome.h"

using namespace std;

NeuralNetwork::NeuralNetwork(const Genome &genome) : genome(genome)
{
    auto genes = genome.getGenes();
    int inputs = genome.getInputs();
    int outputs = genome.getOutputs();

    sort(genes.begin(), genes.end(), [] (Gene g1, Gene g2) -> bool {return g1.getTo() < g2.getTo();});

    for (int i = 0; i < inputs; ++i)
    {
        NeuralNode node(this);
        node.makeInputNode();
        nodes.insert(std::map<int, NeuralNode>::value_type(i,node));
        inputNodes.insert(std::map<int, NeuralNode*>::value_type(i,&nodes.find(i)->second));
    }

    NeuralNode biasNode(this);
    biasNode.makeInputNode();
    nodes.insert(std::map<int, NeuralNode>::value_type(inputs, biasNode));

    for (int i = 0; i < outputs; i++)
    {
        nodes.insert(std::map<int, NeuralNode>::value_type(MAX_NEURONS+i,NeuralNode(this)));
        outputNodes.insert(std::map<int, NeuralNode*>::value_type(MAX_NEURONS+i,&nodes.find(MAX_NEURONS+i)->second));
    }

    for (auto it = genes.begin(); it != genes.end(); it++)
    {
        if (!nodes.count(it->getTo()))
            nodes.insert(std::map<int, NeuralNode>::value_type(it->getTo(),NeuralNode(this)));
        nodes.find(it->getTo())->second.addIncoming(*it);
        if (!nodes.count(it->getFrom()))
            nodes.insert(std::map<int, NeuralNode>::value_type(it->getFrom(),NeuralNode(this)));
    }
}

void NeuralNetwork::reset()
{
    for (auto it = nodes.begin(); it != nodes.end(); it++)
    {
        it->second.reset();
    }
}

std::vector<double> NeuralNetwork::evaluate(std::vector<double> inputs)
{
    if (inputs.size() != inputNodes.size())
        __throw_runtime_error("Inputs are not of right size");

    reset();

    auto it1 = inputs.begin();
    auto it2 = inputNodes.begin();
    for (; it1 != inputs.end() && it2 != inputNodes.end(); it1++, it2++)
    {
        it2->second->setInput(*it1);
    }

    auto iter = nodes.begin();

    for (int i = 0; i < genome.getInputs()+1; i++, iter++)
    {}

    for (; iter != nodes.end(); iter++)
    {
        iter->second.evaluate();
    }

    vector<double> result = vector<double>();

    for (auto it = outputNodes.begin(); it != outputNodes.end(); it++)
    {
        result.push_back(it->second->getValue());
    }

    return result;
}

const NeuralNode &NeuralNetwork::getNode(int node) const
{
    return nodes.find(node)->second;
}

