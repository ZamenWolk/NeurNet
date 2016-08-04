//
// Created by mcn on 7/26/16.
//

#ifndef NEURNET_NEURALNETWORK_H
#define NEURNET_NEURALNETWORK_H

#include <map>
#include "NeuralNode.h"

class Genome;

class NeuralNetwork
{
public:
    friend class Genome;

    void reset();

    std::vector<double> evaluate(std::vector<double> inputs);

    const NeuralNode & getNode(int node) const;

private:
    NeuralNetwork(const Genome &genome);

    const Genome &genome;
    std::map<int, NeuralNode> nodes;
    std::map<int, NeuralNode*> inputNodes;
    std::map<int, NeuralNode*> outputNodes;
};


#endif //NEURNET_NEURALNETWORK_H
