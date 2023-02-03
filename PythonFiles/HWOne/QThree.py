"""
3. A cricket randomly hops between 4 leaves, on each turn hopping to one of the
other 3 leaves with equal probability. After n  ≥ 0 hops, what is the probability
that  the  cricket  has  returned  to  the  leaf  where  it  started?  Design  a  recursive
algorithm  to  solve  this  problem.  You  need  to  give  the  main  ideas  of  your
algorithm and the recursive formula.

Answer:
The probability that the cricket returns to the leaf that it started on after n hops is given by:

P(original, original, n) = 1/3*(1-P(original, original, n-1))
^ One third times the probability that it is not on the starting leaf after n-1
OR:
P(original, original, n) = 1/3*(P(original,notOrig1, n-1) + P(original,notOrig2, n-1) + P(original, notOrig3, n-1))
^ One third times the probability that it is not on the starting leaf after n-1

Since the former will be easier to calculate in the equal probability case, I plan on generating that as an answer.

A function that implements that logic:
function P(n)
    if n is 0:
        return 1
    return 1/3 * (1 - p(n-1))

This algorithm essentially converges onto 1/(number of leaves) and could be easily modified to handle any number
of leaves, even with different probabilities of landing on each.

In this case, the function converges onto 1/4.
"""

def p(n, numberOfLeaves=4):  # Works with any number of leaves
    if n == 0:
        return 1
    prob = p(n - 1, numberOfLeaves)
    print(str(1 / (numberOfLeaves-1)) + " * (" + str(1) + "-" + str(prob) + ") = " + str(1 / (numberOfLeaves-1) * (1 - prob)))
    return 1 / (numberOfLeaves-1) * (1 - prob)


if __name__ == "__main__":
    p(10)

