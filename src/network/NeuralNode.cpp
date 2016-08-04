//
// Created by mcn on 7/26/16.
//

#include <cmath>
#include "NeuralNode.h"
#include "NeuralNetwork.h"
#include "../util/util.h"

using namespace std;
using namespace std::placeholders;

void NeuralNode::addIncoming(Gene &gene)
{
    incoming.push_back(gene);
}

NeuralNode::NeuralNode(NeuralNetwork *network) : network(network), inputFunction(nullptr), value(), calculated(false)
{

}

NeuralNode &NeuralNode::operator=(NeuralNode &&node)
{
    this->network = node.network;
    this->incoming = node.incoming;
    this->value = node.value;
    this->calculated = node.calculated;

    if (node.inputFunction)
        this->inputFunction = NULL;
    else
        this->inputFunction = bind(&NeuralNode::setValue, this, _1);

    return *this;
}

NeuralNode &NeuralNode::operator=(const NeuralNode &node)
{
    this->network = node.network;
    this->incoming = node.incoming;
    this->value = node.value;
    this->calculated = node.calculated;

    if (node.inputFunction)
        this->inputFunction = NULL;
    else
        this->inputFunction = bind(&NeuralNode::setValue, this, _1);

    return *this;
}

NeuralNode::NeuralNode(const NeuralNode &node)
{
    this->network = node.network;
    this->incoming = node.incoming;
    this->value = node.value;
    this->calculated = node.calculated;

    if (node.inputFunction)
        this->inputFunction = NULL;
    else
        this->inputFunction = bind(&NeuralNode::setValue, this, _1);
}

double NeuralNode::getValue() const
{
    return value;
}

void NeuralNode::reset()
{
    value = 0;
    calculated = false;
}

void NeuralNode::setInput(double input)
{
    inputFunction(input);
}

void NeuralNode::makeInputNode()
{
    inputFunction = std::bind(&NeuralNode::setValue, this, _1);
}

bool NeuralNode::evaluate()
{
    double sum = 0;

    if (calculated)
        return calculated;

    for (auto it = incoming.begin(); it != incoming.end(); it++)
    {
        const NeuralNode &incomingNode = network->getNode(it->getFrom());
        if (!incomingNode.isCalculated())
            return false;
        sum += incomingNode.getValue() * it->getWeight();
    }

    value = ACTIVATION_FUNCTION(sum);

    calculated = true;
    return calculated;
}

void NeuralNode::setValue(double input)
{
    value = input;
    calculated = true;
}

bool NeuralNode::isCalculated() const
{
    return calculated;
}
