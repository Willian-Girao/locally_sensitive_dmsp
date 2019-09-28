# Experimental Methods Results
Best results amongst the methods are shown in bold (both path length and # of messages exchanged). The shapes (hull and alpha) are calculated, at each node the mule passes through, using {u} U N(u) (called here **N***): N* <span>&#x2287;</span> **alpha-shape**  <span>&#x2287;</span> **convex-hull**. Thus, it is expected that the method based on the **convex-hull** of **N*** will have **solutions either as good as or worse than the method based on the greedy approach** (and will send less messages), while the method based on the  **<span>&#x3B1;</span>-shape** of **N*** will have **solutions either equal or better than the ones found by the convex-hull, but at most as good as the greedy approach** (with less messages than the greedy one, and a fewer more than the convex-hull one). 

The experimental results listed bellow verifies this expected behavior. The **<span>&#x3B1;</span>** utilized to calculate the <span>&#x3B1;</span>-shape of N* equals the **diameter** of the node's communication range (same for all nodes within the same graph).

## Mule Movements

As expected, the computational experiments show that the method based on the concept of <span>&#x3B1;</span>-shapes, in the worst case, have the same solutions as the convex-hull, while in some cases it is able to match the greedy solutions.

 instance  | Greedy | Convex-hull | <span>&#x3B1;</span>-shape
--------------- | --------------- | ---------------| ---------------
kroD100 | 6 | 6 | 6
rat195 | 4 |  4  |  4 
team2_201 | 10 | 12 | 10 
team3_301 | 74 | 76 | 76 
lin318 | 8 |  8 | 8 
rd400 | 14  | 14 | 14
pcb442 | 12 | 12 | 12
team6_501 | 6 | 6 |  6
dsj1000 | X | X | X 
bonus1000 | X | X | X

## Messages Exchanged

The outcome for the messaging too follows the expected behavior, considering the structure of the shapes. 

instance  | Greedy | Convex-hull | <span>&#x3B1;</span>-shape
--------------- | --------------- | ---------------| ---------------
kroD100 | 11084 | 10518 | 10634
rat195 | 42990 | 42064 |42206
team2_201 |  30522 | 29002 | 30052
team3_301 |  21446 | 19240 | 19552
lin318 |  94218 |  91670 | 93706
rd400| 112638 | 108372 |109382
pcb442 | 169902 | 164752 | 165554
team6_501 | 312228 | 307380 | 308322
dsj1000 | X | X | X 
bonus1000 | X | X | X 