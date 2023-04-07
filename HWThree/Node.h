//
// Created by matts on 3/2/2023.
//

#ifndef ECE5330_NODE_H
#define ECE5330_NODE_H

#include <vector>
#include <cstdint>

class Node
{
public:
    Node();
    explicit Node(int nodeNumber, int numberOfAdjNodes);
    ~Node();

    std::vector<Node*> getAdjNodes() {return adjNodes;};
    std::vector<int> getWeights() {return weights;};
    [[nodiscard]] int getDistance() const {return distance; }
    [[nodiscard]] int getNumberOfAdjNodes() const {return numberOfAdjNode;};
    [[nodiscard]] int getNodeNumber() const {return nodeNumber;};
    Node* getClosestNode() { return this->closestNode;}

    void setDistance(int pathDist, Node* from) { this->distance = pathDist; this->closestNode = from; }

    void addAdjNode(Node* node, int weight);

private:
    int nodeNumber = 0;
    int numberOfAdjNode = 0;
    int distance = INT32_MAX;
    Node* closestNode = nullptr;
    std::vector<Node*> adjNodes;
    std::vector<int> weights;
};


#endif //ECE5330_NODE_H
