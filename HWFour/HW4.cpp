#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <fstream>
#include <chrono>

class Node
{
public:
    Node();

    explicit Node(int nodeNumber, int numberOfAdjNodes) : numberOfAdjNode(numberOfAdjNodes)
        {this->nodeNumber = nodeNumber;}

    ~Node()
    {
        for(int i = 0; i < numberOfAdjNode; i++) {delete adjNodes.at(i);}
        this->adjNodes.clear();
        this->weights.clear();
    }

    std::vector<Node*> getAdjNodes() {return adjNodes;}
    std::vector<int> getWeights() {return weights;}
    [[nodiscard]] int getDistance() const {return distance; }
    [[nodiscard]] int getNumberOfAdjNodes() const {return numberOfAdjNode;}
    [[nodiscard]] int getNodeNumber() const {return nodeNumber;}
    Node* getClosestNode() { return this->closestNode;}

    void setDistance(int pathDist, Node* from) { this->distance = pathDist; this->closestNode = from; }

    void addAdjNode(Node* node, int weight)
    {
        adjNodes.push_back(node);
        weights.push_back(weight);
        numberOfAdjNode++;
    }

private:
    int nodeNumber = 0;
    int numberOfAdjNode = 0;
    int distance = INT32_MAX;
    Node* closestNode = nullptr;
    std::vector<Node*> adjNodes;
    std::vector<int> weights;
};

class NodeHeap
{
public:
    explicit NodeHeap(std::vector<Node*> nodes) {this->heapVec = std::move(nodes);}
    ~NodeHeap();

    [[nodiscard]] size_t getSize() const {return heapVec.size()-1;};
    [[nodiscard]] bool isEmpty() const {return this->getSize() == 0;};

    Node* deleteMin()
    {
        std::swap(this->heapVec.at(0), this->heapVec.at(this->heapVec.size()-1)); // Root node = Last leaf
        Node* minNode = this->heapVec.at(this->heapVec.size()-1);
        this->heapVec.pop_back();
        int nextCheck = 0; // Apply percolateDown on root node and subsequent nodes
        while(nextCheck != -1)
        {
            nextCheck = percolateDown(nextCheck);
        }
        return minNode;
    }

    int percolateDown(int index)
    {
        if(index <= (int) getSize() / 2) // Index will go out of bounds and break the program
        {
            int leftVal = index*2+1 <= getSize() ? this->heapVec.at(index * 2+1)->getDistance() : INT32_MAX;
            int rightVal = index*2+2 <= getSize() ? this->heapVec.at(index* 2+2)->getDistance() : INT32_MAX;
            int swapWith = leftVal < rightVal ? index*2+1 : index*2+2;
            if(this->heapVec.size() > swapWith && this->heapVec.at(index)->getDistance() > this->heapVec.at(swapWith)->getDistance())
            {
                std::swap(this->heapVec.at(index), this->heapVec.at(swapWith)); // Root node = Last
                return swapWith;
            }
            else {return -1;}
        }
        else {return -1;}
    }

    void percolateUp(Node* node)
    {
        int index = 0;
        for(int i = 0; i < heapVec.size(); i++)
        {
            if(heapVec.at(i) == node){
                index = i;
                break;
            }
        }
        int parent = (int) ((index - 1) / 2);
        while(parent >= 0 && this->heapVec.at(index)->getDistance() < this->heapVec.at(parent)->getDistance())
        {
            std::swap(this->heapVec.at(index), this->heapVec.at(parent)); // Index node = parent
            index = parent;
            parent = (int) (index-1) / 2;
        }
    }

    void buildHeap()
    {
        for(int i = (int) getSize()/2; i >= 0; i--){ // Start at the last node with leaves
            int nextCheck = i;
            while(nextCheck != -1){
                nextCheck = percolateDown(nextCheck); // Look at node where the large element just went
            }
        }
    }

    void checkHeapOrder()
    {
        for(int i = 0; i < getSize(); i++)
        {
            if(i*2+1 < getSize())
            {
                if(this->heapVec.at(i)->getDistance() > this->heapVec.at(i*2+1)->getDistance())
                {
                    std::cout << "Heap order violated at index " << i << std::endl;
                }
            }
            if(i*2+2 < getSize())
            {
                if(this->heapVec.at(i)->getDistance() > this->heapVec.at(i*2+2)->getDistance())
                {
                    std::cout << "Heap order violated at index " << i << std::endl;
                }
            }
        }
    }

private:
    std::vector<Node*> heapVec;
};

class Graph
{
public:
    explicit Graph() = default;

    ~Graph()
    {
        for(auto & node : nodes) {delete node;}
        nodes.clear();
    }

    void addNode(int nodeNumberOne)
    {
        Node* node = new Node(nodeNumberOne, 0);
        nodes.push_back(node);
    };

    void addEdge(int nodeNumberOne, int nodeNumberTwo, int weight) {
        nodes.at(nodeNumberOne)->addAdjNode(nodes.at(nodeNumberTwo), weight);
        nodes.at(nodeNumberTwo)->addAdjNode(nodes.at(nodeNumberOne), weight);
    }

    void addEdgeDirectedFromTo(int nodeNumberOne, int nodeNumberTwo, int weight) {
        nodes.at(nodeNumberOne)->addAdjNode(nodes.at(nodeNumberTwo), weight);
    }

    int dijkstras(int start, int end)
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

            // Return distance if end node is found
            if(u->getNodeNumber() == endNode->getNodeNumber())
            {
                std::cout << printShortestPath(endNode) << "\n";
                return u->getDistance();
            }

            // Return -1 if min node with MAX distance is found --> something is wrong or disconnected graph
            if(u->getDistance() == INT32_MAX)
            {
                std::cout << "Path does not exist.\n";
                return -1;
            }

            // Add node to the shortest paths tree
            for(int i = 0; i < u->getNumberOfAdjNodes(); i++)
            {
                int uVDist = u->getDistance() + u->getWeights().at(i);
                Node* adjNode = u->getAdjNodes().at(i);

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

    int bellmanFord(int start, int end){
        // Define Start/Stop Nodes
        Node* startNode = this->nodes[start];
        Node* endNode = this->nodes[end];

        // Set all nodes except start to have distance of max
        for(auto* node : this->nodes)
        {
            node->setDistance(INT32_MAX, nullptr);
        }
        startNode->setDistance(0, nullptr);

        bool* noCurrentChange = new bool[this->nodes.size()-1]();
        bool* noPastChange = new bool[this->nodes.size()-1]();

        for(int i = 0; i < this->nodes.size(); i++){
            for(auto* node: this->nodes){
                if(!noPastChange[node->getNodeNumber()] && !node->getAdjNodes().empty())
                {
                    for (int j = 0; j < node->getAdjNodes().size(); j++)
                    {
                        if (node->getDistance() != INT32_MAX && node->getAdjNodes().at(j)->getDistance() > node->getDistance() + node->getWeights().at(j))
                        {
                            node->getAdjNodes().at(j)->setDistance(node->getDistance() + node->getWeights().at(j),node);
                            noCurrentChange[node->getAdjNodes().at(j)->getNodeNumber()] = false;
                        }
                    }
                }
            }

            // Check if all the elements in noCurrentChange are true
            bool allTrue = true;
            for(int j = 0; j < this->nodes.size(); j++){
                if(!noCurrentChange[j]){
                    allTrue = false;
                }
            }

            // Break if all elements in noCurrentChange are true (no changes in last iteration)
            if(allTrue){
                break;
            }
            else if(i == this->nodes.size()-1){ // Check the nth iteration for a change that would indicate a negative cycle
                std::cout << "Negative Cycle Detected.\n";
                return INT32_MAX;
            }

            //Copy and reset noCurrentChange
            for(int j = 0; j < this->nodes.size(); j++){
                noPastChange[j] = noCurrentChange[j];
                noCurrentChange[j] = true;
            }
        }
        std::cout << printShortestPath(endNode) << "\n";
        return endNode->getDistance();
    }


    std::string toString()
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

    std::string toAdjacencyListString()
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

    static std::string printShortestPath(Node* node)
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

private:
    std::vector<Node*> nodes;
};

int main(){
    std::cout << "HW4" << "\n";
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

    // Read in source
    std::cout << "Please input source node:\n";
    std::string source;
    std::cin >> source;

    // Read in sink
    std::cout << "Please input sink node:\n";
    std::string sink;
    std::cin >> sink;

    int sourceInt = std::stoi(source);
    int sinkInt = std::stoi(sink);

    if(!(sourceInt >= 0 && sourceInt <= numberOfNodes && sinkInt >= 0 && sinkInt <= numberOfNodes))
    {
        std::cerr << "Input number is invalid.";
        return -1;
    }

    // Create graphUndirected
    auto* graphDirected = new Graph();

    // Add nodes to the graphUndirected
    std::getline(inFile, buffer);
    for(int i = 0; i < numberOfNodes; i++){
        std::getline(inFile, buffer);
        graphDirected->addNode(std::stoi(buffer));
    }

    // Read in edges and add to the graphUndirected
    while(!inFile.eof()){
        // Split the string delimited by a spaces
        inFile >> buffer;
        int nodeOne = std::stoi(buffer);
        inFile >> buffer;
        int nodeTwo = std::stoi(buffer);
        inFile >> buffer;
        int weight = std::stoi(buffer);

        // Add edge to the graphUndirected
        graphDirected->addEdgeDirectedFromTo(nodeOne, nodeTwo, weight);
    }
    // Close the file
    inFile.close();
    int shortestPath;

    std::cout << "\nRunning Dijkstras on graph with negative edge weight:\n";
    shortestPath = graphDirected->dijkstras(sourceInt, sinkInt);
    std::cout << "Shortest Path Length: " << shortestPath << "\n";

    std::cout << "\nRunning Bellman-Ford on same graph:\n";
    shortestPath = graphDirected->bellmanFord(sourceInt, sinkInt);
    std::cout << "Shortest Path Length: " << shortestPath;
}



