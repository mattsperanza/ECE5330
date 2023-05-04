#include <iostream>
#include <utility>
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
        this->adjNodes.clear();
        this->weights.clear();
    }

    std::vector<Node*> getAdjNodes() {return adjNodes;}
    std::vector<int> getWeights() {return weights;}
    void setWeights(std::vector<int> newWeights) {this->weights = std::move(newWeights);}
    void setWeight(int index, int value) {this->weights[index] = value;}
    void setAdjNodes(std::vector<Node*> newNodes) { this->adjNodes = newNodes; }
    [[nodiscard]] int getDistance() const {return distance; }
    [[nodiscard]] int getNumberOfAdjNodes() const {return adjNodes.size();}
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

    void setLoad(int i){this->load = i;}
    [[nodiscard]] int getLoad() const {return this->load;}

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
    explicit Graph(std::vector<Node*> nodes){ this->nodes = std::move(nodes); }

    ~Graph()
    {
        for(auto* node : nodes) {delete node;}
        for(auto* node: savedState) { delete node; }
        nodes.clear();
        savedState.clear();
    }

    std::vector<Node*> getNodes() {return nodes;}

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

    void saveState(){
        for(int i = 0; i < this->nodes.size(); i++){
            Node* current = nodes[i];
            Node* newNode = new Node(i, current->getNumberOfAdjNodes());
            this->savedState.push_back(newNode);
            newNode->setWeights(current->getWeights());
            newNode->setAdjNodes(current->getAdjNodes());
        }
        for(auto current : this->savedState)
        {
            auto* newAdjNodes = new std::vector<Node*>();
            for(int j = current->getAdjNodes().size()-1; j >= 0; j--)
            {
                int nodeNum = current->getAdjNodes()[j]->getNodeNumber();
                newAdjNodes->push_back(this->savedState[nodeNum]);
            }
            current->setAdjNodes(*newAdjNodes);
        }
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
            Node* current = remaining->front();
            std::vector<Node*> adjNodes = current->getAdjNodes();
            for(int i = 0; i < adjNodes.size(); i++){
                if(adjNodes[i] == sinkNode){
                    adjNodes[i]->setDistance(current->getWeights()[i], current);
                    // Create short path list
                    auto* path = new std::vector<Node*>;
                    path->push_back(sinkNode);
                    Node* back = adjNodes[i]->getClosestNode();
                    assert(back == current);
                    path->push_back(back);
                    while(back->getClosestNode() != nullptr){
                        back = back->getClosestNode();
                        path->push_back(back);
                    }
                    return path; // Path is in revers order in the vector
                }
                if(!visited[adjNodes[i]->getNodeNumber()]) {
                    adjNodes[i]->setDistance(current->getWeights()[i], current);
                    remaining->push_back(adjNodes[i]);
                    visited[adjNodes[i]->getNodeNumber()] = true;
                }
            }
            remaining->remove(remaining->front());
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
            Node* current = remaining->front();
            std::vector<Node*> adjNodes = current->getAdjNodes();
            for(int i = 0; i < adjNodes.size(); i++){
                if(!visited[adjNodes[i]->getNodeNumber()]) {
                    adjNodes[i]->setDistance(current->getWeights()[i], current);
                    remaining->push_back(adjNodes[i]);
                    visited[adjNodes[i]->getNodeNumber()] = true;
                }
            }
            remaining->remove(remaining->front());
        }

        auto* visitTree = new std::vector<Node*>;
        for(auto* n: G->nodes){
            if(visited[n->getNodeNumber()]){
                visitTree->push_back(n);
            }
        }
        return visitTree;
    }

    static std::vector<Node*> getFlow(std::vector<Node *> initialNet, std::vector<Node *> residualNet) {
        for(auto* node: initialNet){
            int nodeNum1 = node->getNodeNumber();
            for(int i = 0; i< node->getAdjNodes().size(); i++){
                int nodeNum2 = node->getAdjNodes()[i]->getNodeNumber();
                for(int j = 0; j < residualNet[nodeNum2]->getAdjNodes().size(); j++)
                {
                    if(residualNet[nodeNum2]->getAdjNodes()[j]->getNodeNumber() == nodeNum1){
                        int reverseWeight = residualNet[nodeNum2]->getWeights()[j];
                        node->setWeight(i, reverseWeight);
                        break; // Found self, now look at other adj nodes
                    }
                }
            } // Looked through all adj nodes and assigned weights
        } // Looked through all nodes in G

        return initialNet;
    }

    static std::vector<Node*> fordFulkerson(Graph* G, Node* sourceNode, Node* sinkNode){
        if (sourceNode == nullptr || sinkNode == nullptr || G->nodes.empty()){
            std::cerr << "Error: Source/Sink/Graph invalid." << "\n";
            return {}; // Return empty vector
        }

        G->saveState();
        auto* path = BFS(G, sourceNode, sinkNode);
        while(!path->empty()){
            int pathFlow = INT32_MAX;
            for(Node* n: *path){
                if(n->getDistance() < pathFlow){
                    pathFlow = n->getDistance();
                }
            }

            for(int i = 0; i < path->size()-1; i++)
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

                int flowFromTwo = nodeTwo->getWeights()[twoToOneAdjNodeIndex];
                if(flowFromTwo == pathFlow){  // Edge needs to be flipped
                    if (oneToTwoAdjNodeIndex == -1) { // Edge doesn't exist
                        G->removeEdgeFromTo(nodeTwo->getNodeNumber(), nodeOne->getNodeNumber());
                        G->addEdgeDirectedFromTo(nodeOne->getNodeNumber(), nodeTwo->getNodeNumber(), pathFlow);
                    }
                    else{ // Edge exists
                        nodeOne->setWeight(oneToTwoAdjNodeIndex, nodeOne->getWeights()[oneToTwoAdjNodeIndex] + pathFlow);
                        G->removeEdgeFromTo(nodeTwo->getNodeNumber(), nodeOne->getNodeNumber());
                    }
                    assert(nodeOne->getClosestNode() == nodeTwo);
                    nodeOne->setDistance(INT32_MAX, nullptr); // Closest node no longer known
                }
                else if(flowFromTwo > pathFlow){
                    if(oneToTwoAdjNodeIndex != -1){  // Shift flow from one edge to another
                        nodeOne->setWeight(oneToTwoAdjNodeIndex, nodeOne->getWeights()[oneToTwoAdjNodeIndex] + pathFlow);
                        nodeTwo->setWeight(twoToOneAdjNodeIndex, nodeTwo->getWeights()[twoToOneAdjNodeIndex] - pathFlow);
                    }
                    else{  // Shift flow from one edge to a newly created edge
                        G->addEdgeDirectedFromTo(nodeOne->getNodeNumber(), nodeTwo->getNodeNumber(), pathFlow);
                        nodeTwo->setWeight(twoToOneAdjNodeIndex, nodeTwo->getWeights()[twoToOneAdjNodeIndex] - pathFlow);
                    }
                }
                else{
                    std::cerr << "Error: Flow from node " << nodeTwo->getNodeNumber() << " is less than path flow." << "\n";
                }
            }
            path = BFS(G, sourceNode, sinkNode);
        }

        int flowValue = 0;
        for (int f: sinkNode->getWeights()){
            flowValue += f;
        }
        std::cout << "Total flow through the graph: " << flowValue << "\n";

        auto* minimumCut = BFSTree(G, sourceNode);
        std::cout << "Minimum cut set S: ";
        for (auto* n: *minimumCut){
            std::cout << n->getNodeNumber() << " ";
        }
        std::cout << "\n";

        //std::cout << "Residual Network (edges with zero weight don't exist): \n";
        //std::cout << G->toString();

        return getFlow(G->savedState, G->nodes);
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

    std::string printFlow()
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
    std::vector<Node*> savedState;
};

int main(){
    std::cout << "HW6" << "\n";
    // Ask user for input file
    std::cout << "Please enter the name of the input file:\n";
    std::string fileName; //= "sampleInput.txt";
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

    // Read in source and sink nodes from user
    std::cout << "Please enter the source node:\n";
    int sourceInt; //= 0;
    std::cin >> sourceInt;
    std::cout << "Please enter the sink node:\n";
    int sinkInt; // = 7;
    std::cin >> sinkInt;

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

    // Print the source and sink node numbers
    std::cout << "Ford-Fulkerson (technically is Edmonds-Karp because I use BFS): \n";
    std::cout << "Source: " << sourceInt << "\nSink: " << sinkInt << "\n";

    // Run Ford Fulkerson
    std::vector<Node*> flow = Graph::fordFulkerson(graphDirected, graphDirected->getNodes().at(sourceInt),
                                                           graphDirected->getNodes().at(sinkInt));
    auto* graphFlow = new Graph(flow);
    std::cout << "Graph Flow:\n" << graphFlow->toString() << std::endl;
}



