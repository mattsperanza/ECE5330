//
// Created by matts on 3/2/2023.
//

#include "Node.h"

Node::Node(int nodeNumber, int numberOfAdjNodes) : numberOfAdjNode(numberOfAdjNodes)
{
    this->nodeNumber = nodeNumber;
}

Node::~Node()
{
    for(int i = 0; i < numberOfAdjNode; i++)
    {
        delete adjNodes.at(i);
    }
    this->adjNodes.clear();
    this->weights.clear();
}

void Node::addAdjNode(Node* node, int weight)
{
    adjNodes.push_back(node);
    weights.push_back(weight);
    numberOfAdjNode++;
}

Node::Node(){}
