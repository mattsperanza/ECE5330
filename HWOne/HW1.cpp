//
// Created by matts on 1/29/2023.
//
#include <iostream>
#include <fstream>
#include "Heap.h"

int main()
{
    //Read in file that starts with the file size
    std::ifstream inFile;
    inFile.open("arrayInput.txt", std::ios::in);
    int size = 0;
    inFile >> size;
    int* array = new int[size];
    for(int i = 0; i < size; i++)
    {
        inFile >> array[i];
    }

    //Build heap of size n as a binary tree
    Heap heap(size, array);
    delete[] array;

    //Test buildHeap
    std::cout << "Heap before buildHeap: \n" << heap.printHeap() << std::endl;
    heap.buildHeap();
    std::cout << "Heap after buildHeap: \n" << heap.printHeap() << std::endl;

    //Check that heap was built correctly
    std::cout << "Checking heap order: \n" << std::endl;
    heap.checkHeapOrder();

    //Test deleteMin by deleting the heap and output to file
    std::cout << "Deleting heap and printing to file (heap should print in order): " << std::endl;
    std::ofstream outFile;
    outFile.open("./arrayOutput.txt", std::ios::out);
    for(int i = 0; i < size; i++)
    {
        std::string min = std::to_string(heap.deleteMin());
        outFile << min << " ";
        std::cout << min << " ";
    }

    //Show that the heap is empty
    std::cout << "\nHeap after Deletion:\n" << heap.printHeap() << std::endl;
    return 0;
}

