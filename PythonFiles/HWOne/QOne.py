"""
1. Give  an  algorithm  to  find  all  nodes  less  than  some  value,  X,  in  a  binary  heap.
Your  algorithm  should  run  in  O(K),  where  K  is  the  number  of  nodes  output.
You  need  to  give  the  main  ideas  of  your  algorithm  and  analyze  the  running
time.

Answer:
This algorithm starts at some point in a binary search tree and checks the head node for the
given condition. If the head node meets the condition, the children are recursively searched
(if they are not null). When the algorithm finds a node with no children or a node that does not
meet the less than condition, it returns. A reference to list of nodes is updated with each of
the nodes meeting the condition. Pseudocode for the function is given here:

function findAllLessThan(X, head, listOfNodes):
    if head->value < X: # If the node meets the criteria
        listOfNodes->add(head)
    else: # Heap order property dead end
        return
    if head->children[0] != null:
        findAllLessThan(X, head->children[0], listOfNodes) # Search first child
        if head->children[1] != null: # Heap order property
            findAllLessThan(X, head->children[1], listOfNodes) # Search second child
    return

This recursive algorithm will visit each node at most once, and will, depending on the number given,
end early due to heap order properties. At each node there are at most 3 conditional checks and one
list addition, meaning this algorithm runs with linear scaling with respect to the number of nodes.

"""