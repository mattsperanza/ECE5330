//
// Created by matts on 3/2/2023.
//

#include <iostream>
#include "Graph.h"
#include "NodeHeap.h"

void Graph::addNode(int nodeNumberOne) {
    Node* node = new Node(nodeNumberOne, 0);
    nodes.push_back(node);
}

void Graph::addEdge(int nodeNumberOne, int nodeNumberTwo, int weight) {
    nodes.at(nodeNumberOne)->addAdjNode(nodes.at(nodeNumberTwo), weight);
    nodes.at(nodeNumberTwo)->addAdjNode(nodes.at(nodeNumberOne), weight);
}

Graph::~Graph()
{
    for(auto & node : nodes){
        delete node;
    }
    nodes.clear();
}

int Graph::dijkstras(int start, int end)
{
    // Define Start/Stop Nodes
    Node* startNode = this->nodes[start];
    Node* endNode = this->nodes[end];

    // Set all nodes except start to have distance of max
    for(auto &node : this->nodes)
    {
        node->setDistance(INT32_MAX, nullptr);
    }
    startNode->setDistance(0, nullptr);

    // Priority queue as a heap dependant on distances
    auto* heap = new NodeHeap(this->nodes);
    heap->buildHeap();
    heap->checkHeapOrder();

    // While deleteMin has not deleted all elements
    while(!heap->isEmpty())
    {
        // Add the smallest distance contained in the heap
        Node* u = heap->deleteMin();

        // Return -1 if min node with MAX distance is found --> something is wrong or disconnected graph
        if(u->getDistance() == INT32_MAX)
        {
            std::cerr << "\nSomething is wrong or the graph is disconnected.\n";
            return -1;
        }

        // Add node to the shortest paths tree
        for(int i = 0; i < u->getNumberOfAdjNodes(); i++)
        {
            int uVDist = u->getDistance() + u->getWeights().at(i);
            Node* adjNode = u->getAdjNodes().at(i);

            // Return distance if end node is found
            if(adjNode->getNodeNumber() == endNode->getNodeNumber())
            {
                adjNode->setDistance(uVDist, u);
                std::cout << printShortestPath(endNode) << "\n";
                return uVDist;
            }

            // If this new node created shorter paths than update paths and heap
            if (adjNode->getDistance() > uVDist)
            {
                adjNode->setDistance(uVDist, u);
                heap->percolateUp(adjNode);
            }
        }
    }
    return 0;
}

std::string Graph::toString()
{
    std::string output;
    for(auto & node : nodes){
        output += "Node " + std::to_string(node->getNodeNumber()) + " has " + std::to_string(node->getNumberOfAdjNodes()) + " adjacent nodes.\n";
        for(int j = 0; j < node->getNumberOfAdjNodes(); j++){
            output += std::to_string(node->getNodeNumber()) + "-->"
                    + std::to_string(node->getAdjNodes().at(j)->getNodeNumber()) + " has a weight of " +
                    std::to_string(node->getWeights().at(j)) + ".\n";
        }
        output += "\n";
    }
    return output;
}

std::string Graph::toAdjacencyListString()
{
    std::string output;
    for(auto & node : nodes){
        output += std::to_string(node->getNodeNumber()) + " --> ";
        for(int j = 0; j < node->getNumberOfAdjNodes(); j++){
            output += std::to_string(node->getAdjNodes().at(j)->getNodeNumber()) + " ";
        }
        output += "\n";
    }
    return output;
}

std::string Graph::printShortestPath(Node* node)
{
    std::vector<int> path;
    while(node->getClosestNode() != nullptr)
    {
        path.emplace_back(node->getNodeNumber());
        node = node->getClosestNode();
    }
    path.emplace_back(node->getNodeNumber());

    std::string out;
    for(int i = path.size()-1; i >= 0; i--)
    {
        out += i == 0 ? std::to_string(path.at(i)) : std::to_string(path.at(i)) + "-->";
    }
    return out;
}


