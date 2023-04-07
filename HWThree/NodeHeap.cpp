#include <iostream>
#include <utility>
#include "NodeHeap.h"

// Build heap of size n as a binary tree
NodeHeap::NodeHeap(std::vector<Node*> nodes)
{
    this->heapVec = std::move(nodes);
}

// Define a destructor for a heap
NodeHeap::~NodeHeap(){}

// Delete the smallest value of the heap and percolate up
Node* NodeHeap::deleteMin()
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

// Swaps the smallest child with the parent node if it is smaller than the parent
int NodeHeap::percolateDown(int index)
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
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
}

// Builds the heap order
void NodeHeap::buildHeap()
{
    for(int i = (int) getSize()/2; i >= 0; i--){ // Start at the last node with leaves
        int nextCheck = i;
        while(nextCheck != -1){
            nextCheck = percolateDown(nextCheck); // Look at node where the large element just went
        }
    }
}

// Checks that the heap is in order
void NodeHeap::checkHeapOrder()
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

bool NodeHeap::isEmpty() const
{
    return this->getSize() == 0;
}

void NodeHeap::percolateUp(Node* node)
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
