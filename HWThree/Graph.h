//
// Created by matts on 3/2/2023.
//

#ifndef ECE5330_GRAPH_H
#define ECE5330_GRAPH_H

#include <string>
#include "Node.h"

class Graph
{
public:
    explicit Graph(int numberOfNodes) {this->numberOfNodes = numberOfNodes; };
    ~Graph();

    void addNode(int nodeNumber);
    void addEdge(int nodeNumberOne, int nodeNumberTwo, int weight);

    int dijkstras(int start, int end);

    std::string toString();
    std::string toAdjacencyListString();
    static std::string printShortestPath(Node* node);
private:
    int numberOfNodes;
    std::vector<Node*> nodes;
};

#endif //ECE5330_GRAPH_H
