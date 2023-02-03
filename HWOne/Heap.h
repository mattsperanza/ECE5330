#ifndef ECE5330_HEAP_H
#define ECE5330_HEAP_H

class Heap
{
public:
    Heap(int size, const int *array);
    ~Heap();
    int deleteMin();
    //Returns the largest index of heapArray
    [[nodiscard]] int getSize() const {return this->size;};
    int percolateDown(int index);
    void buildHeap();
    void checkHeapOrder();
    std::string printHeap();
private:
    int size = 0;
    int* heapArray;
};


#endif //ECE5330_HEAP_H
