//
// Created by mcn on 7/26/16.
//

#ifndef NEURNET_NEURALNODE_H
#define NEURNET_NEURALNODE_H

#include <functional>
#include <vector>

#include "../storage/Gene.h"

class NeuralNetwork;

class NeuralNode
{
public:

    NeuralNode(NeuralNetwork *network);
    NeuralNode(const NeuralNode &node);
    NeuralNode& operator=(NeuralNode&& node);
    NeuralNode& operator=(const NeuralNode& node);
    void addIncoming(Gene &gene);

    double getValue() const;

    void reset();

    void setInput(double input);

    void makeInputNode();

    bool evaluate();

    bool isCalculated() const;

private:

    std::vector<Gene> incoming;
    NeuralNetwork const *network;
    double value;
    bool calculated;

    std::function<void(double)> inputFunction;

    void setValue(double input);
};


#endif //NEURNET_NEURALNODE_H
