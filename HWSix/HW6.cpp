#include <iostream>
#include <vector>
#include <list>
#include <cstdint>
#include <string>
#include <fstream>
#include <chrono>
#include <algorithm>

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

    void removeAdjNode(Node* node){
        int index = 0;
        for(int i = 0; i < adjNodes.size(); i++){
            if(adjNodes[i] == node){
                index = i;
                break;
            }
        }
        adjNodes.erase(adjNodes.begin()+index);
        weights.erase(weights.begin()+index);
    }

    static bool isDifferent(Node* n1, Node* n2){
        bool different = true;
        different = n1->getNodeNumber() == n2->getNodeNumber() && n1->
        return different;
    }

    Node *copy(){
        auto* copy = new Node(this->getNodeNumber(), this->getAdjNodes().size());
        copy->setDistance(this->getDistance(), nullptr);
        return copy;
    };

private:
    int nodeNumber = 0;
    int numberOfAdjNode = 0;
    int distance = INT32_MAX;
    Node* closestNode = nullptr;
    std::vector<Node*> adjNodes;
    std::vector<int> weights;
    int load = 0;
};

class NodeHeap
{
public:
    // Vector that points at the same nodes as the Graph nodes, but is a copy of the Graph.nodes vector
    // Can edit nodes, but not Graph.nodes
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
    }

    void addNode(Node* node){
        nodes.push_back(node);
    }

    void addEdge(int nodeNumberOne, int nodeNumberTwo, int weight) {
        nodes.at(nodeNumberOne)->addAdjNode(nodes.at(nodeNumberTwo), weight);
        nodes.at(nodeNumberTwo)->addAdjNode(nodes.at(nodeNumberOne), weight);
    }

    void addEdgeDirectedFromTo(int nodeNumberOne, int nodeNumberTwo, int weight) {
        nodes.at(nodeNumberOne)->addAdjNode(nodes.at(nodeNumberTwo), weight);
    }

    void removeEdgeFromTo(int nodeNumberOne, int nodeNumberTwo){
        nodes.at(nodeNumberOne)->removeAdjNode(nodes.at(nodeNumberTwo));
    }

    // Returns a path in reverse order <sinkNode, ..., sourceNode>
    static std::vector<Node*>* BFS(Graph* G, Node* sourceNode, Node* sinkNode){
        if(sourceNode == nullptr || sinkNode == nullptr){
            auto* failed = new std::vector<Node*>;
            return failed;
        }

        auto* remaining = new std::list<Node*>;
        bool* visited = new bool[G->nodes.size()];
        remaining->push_front(sourceNode);
        visited[0] = true;

        while(!remaining->empty()){
            Node current = *remaining->front();
            for(Node* n: current.getAdjNodes()){
                if(n == sinkNode){
                    n->setDistance(n->getDistance(), &current);
                    // Create short path list
                    auto* path = new std::vector<Node*>;
                    Node* back = n->getClosestNode();
                    path->push_back(back);
                    while(back->getClosestNode() != nullptr){
                        back = back->getClosestNode();
                        path->push_back(back);
                    }
                    return path; // Path is in revers order in the vector
                }
                if(!visited[n->getNodeNumber()]) {
                    n->setDistance(n->getDistance(), &current);
                    remaining->push_back(n);
                    visited[n->getNodeNumber()] = true;
                }
            }
            remaining->pop_front();
        }

        auto* failed = new std::vector<Node*>;
        return failed;
    }

    // Returns a vector of Node* which represents the flow cut
    static std::vector<Node*>* BFSTree(Graph* G, Node* sourceNode){
        if(sourceNode == nullptr){
            auto* failed = new std::vector<Node*>;
            return failed;
        }

        auto* remaining = new std::list<Node*>;
        bool* visited = new bool[G->nodes.size()];
        remaining->push_front(sourceNode);
        visited[sourceNode->getNodeNumber()] = true;

        while(!remaining->empty()){
            Node current = *remaining->front();
            for(Node* n: current.getAdjNodes()){
                if(!visited[n->getNodeNumber()]) {
                    n->setDistance(n->getDistance(), &current);
                    remaining->push_back(n);
                    visited[n->getNodeNumber()] = true;
                }
            }
            remaining->pop_front();
        }

        auto* visitTree = new std::vector<Node*>;
        for(auto* n: G->nodes){
            if(visited[n->getNodeNumber()]){
                visitTree->push_back(n);
            }
        }
        return visitTree;
    }

    static bool isDifferent(Graph* G, Graph* L){
        bool different = true;
        if(G->nodes.size() != L->nodes.size()){
            return false;
        }
        int counter = 0;
        for(int i = 0; i < G->nodes.size(); i++){
            if(!Node::isDifferent(G->nodes[i], L->nodes[i]))
            {
                counter++;
            }
        }
        if(counter == G->nodes.size()-1)
        {
            different = false;
        }

        return different;
    }

    Graph* deepCopy(){
        auto* copy = new Graph();
        auto* listOfNodeNumbers = new std::vector<int>();
        auto* listOfEdgeWeights = new std::vector<int>();
        for(auto & node : this->nodes){
            copy->addNode(node->copy());
            for(int j = 0; j < node->getAdjNodes().size(); j++){
                listOfNodeNumbers->push_back(node->getAdjNodes()[j]->getNodeNumber());
                listOfEdgeWeights->push_back(node->getWeights()[j]);
            }
        }
        int counter = 0;
        for(auto & node : copy->nodes){
            for(int j = 0; j < node->getNumberOfAdjNodes(); j++){
                copy->addEdgeDirectedFromTo(node->getNodeNumber(), listOfNodeNumbers->at(counter), listOfEdgeWeights->at(counter));
                counter++;
            }
        }
        return copy;
    }

    static int fordFulkerson(Graph* G, Node* sourceNode, Node* sinkNode){
        if (sourceNode == nullptr || sinkNode != nullptr || !G->nodes.empty()){
            std::cerr << "Error: Source/Sink/Graph invalid." << "\n";
        }

        auto* originalGraph = G->deepCopy();
        auto* path = BFS(G, sourceNode, sinkNode);
        while(!path->empty()){
            int pathFlow = INT32_MAX;
            for(Node* n: *path){
                if(n->getDistance() < pathFlow){
                    pathFlow = n->getDistance();
                }
            }

            for(int i = 0; i < path->size()-1; i++)  // size-1 to avoid indexing errors
            {
                Node* nodeOne = path->at(i);
                Node* nodeTwo = path->at(i+1);
                int twoToOneAdjNodeIndex = 0;
                int oneToTwoAdjNodeIndex = -1; // Doesn't necessarily exist
                for(int j = 0; j < nodeTwo->getAdjNodes().size(); j++){
                    if (nodeTwo->getAdjNodes()[j] == nodeOne){
                        twoToOneAdjNodeIndex = j;
                        break;
                    }
                }
                for(int j = 0; j < nodeOne->getAdjNodes().size(); j++){
                    if (nodeOne->getAdjNodes()[j] == nodeTwo){
                        oneToTwoAdjNodeIndex = j;
                        break;
                    }
                }

                int flowFromTwo = nodeOne->getDistance();
                if(flowFromTwo == pathFlow){  // Edge needs to be flipped
                    G->removeEdgeFromTo(nodeTwo->getNodeNumber(), nodeOne->getNodeNumber());
                    G->addEdgeDirectedFromTo(nodeOne->getNodeNumber(), nodeTwo->getNodeNumber(), pathFlow);
                }
                else if(flowFromTwo > pathFlow){
                    if(oneToTwoAdjNodeIndex != -1){  // Shift flow from one edge to another
                        nodeOne->getWeights()[oneToTwoAdjNodeIndex] += pathFlow;
                        nodeTwo->getWeights()[twoToOneAdjNodeIndex] -= pathFlow;
                    }
                    else{  // Shift flow from one edge to a newly created edge
                        G->addEdgeDirectedFromTo(nodeOne->getNodeNumber(), nodeTwo->getNodeNumber(), pathFlow);
                        nodeTwo->getWeights()[twoToOneAdjNodeIndex] -= pathFlow;
                    }
                }
            }
            path = BFS(G, sourceNode, sinkNode);
        }

        int flowValue = 0;
        for (int f: sinkNode->getWeights()){
            flowValue += f;
        }

        auto* minimumCut = BFSTree(G, sourceNode);


        return flowValue;
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
        for(int i = (int) path.size()-1; i >= 0; i--)
        {
            out += i == 0 ? std::to_string(path.at(i)) : std::to_string(path.at(i)) + "-->";
        }
        return out;
    }

private:
    std::vector<Node*> nodes;
};

int main(){
    std::cout << "HW6" << "\n";
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

    int sourceInt = 0;
    int sinkInt = 7;

    // Create graphDirected
    auto* graphDirected = new Graph();

    // Add nodes to the graphDirected
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
        int capacity = std::stoi(buffer);

        // Add edge to the graphDirected
        graphDirected->addEdgeDirectedFromTo(nodeOne, nodeTwo, capacity);
    }



    // Close the file
    inFile.close();
}



