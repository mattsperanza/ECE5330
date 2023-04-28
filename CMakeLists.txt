cmake_minimum_required(VERSION 3.23.2)
project(ECE5330)

set(CMAKE_CXX_STANDARD 17)

add_executable(ECE5330_HW1 HWOne/HW1.cpp HWOne/Heap.cpp HWOne/Heap.h)

add_executable(ECE5330_HW3 HWThree/HW3.cpp HWThree/Graph.cpp HWThree/Node.cpp HWThree/Graph.h HWThree/Node.h
        HWThree/NodeHeap.cpp HWThree/NodeHeap.h)

add_executable(ECE5330_HW4 HWFour/HW4.cpp)

add_executable(ECE5330_HW6 HWSix/HW6.cpp)