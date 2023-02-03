"""
2. In  our  class,  we  have  shown  that  an  inorder  traversal  of  a  binary  tree  can  be
implemented  with  recursion.  Please  write  a  pseudo-code  to  implement  the
inorder traversal without using recursion.  You  need  to  choose  an  appropriate
data structure reviewed in class.

Answer:
Assume the binary tree is stored in an array with the following properties discussed in class:
    For any element in array position j (starting at 1) -
1. The left child is in position 2j
2. The right child is in the cell after the left child 2j+1
3 the parent is in the position floor(j/2)

With this structure, we can now think of an algorithm that will follow in-order traversal of a binary
tree, visiting all child nodes from left to right. This algorithm does assume that the root node has
two children. Modification of this to allow for less than two children would require checking for how
many children it has and breaking based off of that. Tree accesses need to be done with j-1, since
using j would cause the program to go out of bounds.

function inOrderTraversal(binaryArray):
    j = 1
    goRight = True
    while True:
        if j == 0:
            break
        if 2 * j exists and goRight is True:
            j = 2 * j
        else if 2 * j + 1 exists:
            j = 2 * j + 1
            goRight = True
        else:
            if j % 2 == 0:
                j = floor(j / 2)
                goRight = False
            else:
                while j % 2 != 0:
                    j = floor(j / 2)
                j = floor(j/2)
                goRight = False
"""

import numpy as np

if __name__ == "__main__":
    binaryArray = np.random.randint(0, 100, 100)
    print(binaryArray)

    visited = []

    j = 1
    jIsOneCounter = 0
    goRight = True
    while True:
        visited.append(j)
        if j == 0:
            break
        print(j)
        if 2 * j <= len(binaryArray) and goRight is True:
            j = 2 * j
        elif 2 * j + 1 <= len(binaryArray):
            j = 2 * j + 1
            goRight = True
        else:
            if j % 2 == 0:
                j = np.floor(j / 2)
                goRight = False
            else:
                while j % 2 != 0:
                    j = np.floor(j / 2)
                    print(j)
                j = np.floor(j/2)
                goRight = False

    print(len(set(visited)) - 1)  # account for zero

