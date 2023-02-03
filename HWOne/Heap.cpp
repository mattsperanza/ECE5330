#include <iostream>
#include "Heap.h"

// Build heap of size n as a binary tree
Heap::Heap(int size, const int *array)
{
    this->size = size;
    this->heapArray = new int[size+1];
    this->heapArray[0] = -1; // Dummy node
    for(int i = 1; i < size+1; i++) // Simply add all elements to array in order
    {
        this->heapArray[i] = array[i-1];
    }
}

// Define a destructor for a heap
Heap::~Heap()
{
    delete[] this->heapArray;
}

// Delete the smallest value of the heap and percolate up
int Heap::deleteMin()
{
    int min = this->heapArray[1];
    this->heapArray[1] = this->heapArray[getSize()]; // Root node = Last leaf
    this->size--;
    int nextCheck = 1; // Apply percolateDown on root node and subsequent nodes
    while(nextCheck != -1)
    {
        nextCheck = percolateDown(nextCheck);
    }
    return min;
}

// Swaps the smallest child with the parent node if it is smaller than the parent
int Heap::percolateDown(int index)
{
    if(index <= (int) getSize() / 2) // Index will go out of bounds and break the program
    {
        int leftVal = index*2 <= getSize() ? this->heapArray[index * 2] : INT32_MAX;
        int rightVal = index*2+1 <= getSize() ? this->heapArray[index * 2+1] : INT32_MAX;
        int swapWith = leftVal < rightVal ? index*2 : index*2+1;
        if(this->heapArray[index] > this->heapArray[swapWith])
        {
            std::swap(this->heapArray[index], this->heapArray[swapWith]);
        }
        return swapWith;
    }
    else
    {
        return -1;
    }
}

// Builds the heap order
void Heap::buildHeap()
{
    for(int i = (int) getSize()/2; i > 0; i--){ // Start at the last node with leaves
        int nextCheck = i;
        while(nextCheck != -1){
            nextCheck = percolateDown(nextCheck); // Look at node where the large element just went
        }
    }
}

// Checks that the heap is in order
void Heap::checkHeapOrder()
{
    for(int i = 1; i < getSize()+1; i++)
    {
        if(i*2 < getSize()+1)
        {
            if(this->heapArray[i] > this->heapArray[i*2])
            {
                std::cout << "Heap order violated at index " << i << std::endl;
            }
        }
        if(i*2+1 < getSize()+1)
        {
            if(this->heapArray[i] > this->heapArray[i*2+1])
            {
                std::cout << "Heap order violated at index " << i << std::endl;
            }
        }
    }
}

// Returns the heap as a string
std::string Heap::printHeap()
{
    std::string output;
    for(int i = 1; i < getSize()+1; i++)
    {
        output += std::to_string(this->heapArray[i]) + " ";
    }
    return output;
}