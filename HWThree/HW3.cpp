//
// Created by matts on 3/2/2023.
//

#include <iostream>
#include <fstream>
#include "Graph.h"

int main(){
    // Ask user for input file
    std::cout << "Please enter the name of the input file:\n";
    std::string fileName;
    std::cin >> fileName;

    // Open the file
    std::ifstream inFile;
    inFile.open(fileName, std::ios::in);
    std::string buffer;

    // Check if the file is open
    if(!inFile.is_open())
    {
        std::cout << "Error opening file" << std::endl;
        return 0;
    }

    // Read in number of nodes
    std::getline(inFile, buffer);  // Skip title line
    std::getline(inFile, buffer);
    std::cout << "Number of nodes: " << buffer << std::endl;
    int numberOfNodes = std::stoi(buffer);

    // Create graph
    auto* graph = new Graph(numberOfNodes);

    // Add nodes to the graph
    std::getline(inFile, buffer);
    for(int i = 0; i < numberOfNodes; i++){
        std::getline(inFile, buffer);
        graph->addNode(std::stoi(buffer));
    }

    // Read in edges and add to the graph
    while(!inFile.eof()){
        // Split the string delimited by a spaces
        inFile >> buffer;
        int nodeOne = std::stoi(buffer);
        inFile >> buffer;
        int nodeTwo = std::stoi(buffer);
        inFile >> buffer;
        int weight = std::stoi(buffer);

        // Add edge to the graph
        graph->addEdge(nodeOne, nodeTwo, weight);
    }
    // Close the file
    inFile.close();
    int restart = 0;
    while(restart != -1)
    {
        std::cout << graph->toAdjacencyListString();

        std::cout << "Source Node:\n";
        int source;
        std::cin >> source;

        std::cout << "Sink Node:\n";
        int sink;
        std::cin >> sink;

        // Wait for user input
        std::cout << "Press enter to run Dijkstra's shortest path algorithm from " << source << " to " << sink;
        std::cin.ignore();
        std::cin.ignore();

        // Run dijkstras algorithm
        std::cout << "Running dijkstras algorithm" << std::endl;
        std::cout << "Shortest Path:" << std::endl;
        int shortest = graph->dijkstras(source, sink);
        std::cout << "Shortest Path Length: \n" << shortest << std::endl;

        std::cout << "Enter -1 to exit or any other int to continue:\n";
        std::cin >> restart;
    }
}
