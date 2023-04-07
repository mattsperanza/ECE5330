#ifndef ECE5330_HEAP_H
#define ECE5330_HEAP_H

#include <vector>
#include "Node.h"

class NodeHeap
{
public:
    explicit NodeHeap(std::vector<Node*> nodes);
    ~NodeHeap();
    Node* deleteMin();

    //Returns the largest index of heapArray
    [[nodiscard]] size_t getSize() const {return heapVec.size()-1;};
    int percolateDown(int index);
    void percolateUp(Node* node);
    void buildHeap();
    void checkHeapOrder();

    std::string printHeap();

    [[nodiscard]] bool isEmpty() const;

private:
    std::vector<Node*> heapVec;
};


#endif //ECE5330_HEAP_H
