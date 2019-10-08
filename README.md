# Locally Sensitive Data Mule Scheduling Problem (W.I.P.)

Ongoing research project on data management and information retrieval within sensor networks under locally sensitive topology restrictions.

## Explored/Proposed methods

### Greedy
### Convex-Hull

## Instances

The instances utilized here come follows the following structure: the first line of the file gives out the **total number of nodes** within the network (modeled as a graph) as well as the sensor's communication range **radius**, followed by the sensors **coordinates** *(x, y)* separated from their respective **neighbors list** by an empty line (the first value corresponds to the total number of neighbors). An example of such instances is shown bellow, followed by its visual representation.

```cpp
// Example graph
6 r
4 5.6
5.3 4.2
3.2 3.8
4 2
1 3
2 1

2 1 2
1 0
3 0 3 4
2 2 5
2 2 5
2 3 4
```
![](https://github.com/Willian-Girao/locally_sensitive_dmsp/blob/master/misc/plots/graph_example.png)

This work considers the case where all the sensors within the network have the same communication area. Sensor nodes *p* and *q* are said to be neighbors (e.g. vertices have an edge connecting them) if their communication range overlap.

## Results

- [With Acks](https://github.com/Willian-Girao/locally_sensitive_dmsp/tree/master/sensitive_dmsp_solver/results/README.md)
- Without Acks

## Source Codes

The code follows an Object Oriented programming paradigm where each node of is represented as an object of a class that encapsulates the sensor, where the knowledge of a sensor *u* (local sensibility) about the network topology is limited to its neighboring sensors listed on the instance file.

In order to simulate a network of communicating and independet sensors the program utilizes a message sharing interface between processes, where each processe represents a sensor of the network.

The source code files can be found [here](https://github.com/Willian-Girao/locally_sensitive_dmsp/blob/master/sensitive_dmsp_solver/README.md) along with instructions on needed dependencies as well as how to compile and run.

## References

1. [H. Edelsbrunner, D.K., Seidel, R., 1983. On the shape of a set of pointsin the plane. IEEE Transactions on      Information Theory 29, 551 – 559.](https://www.semanticscholar.org/paper/The-Shape-of-a-Set-of-Points-in-the-Plane-Edelsbrunner-Kirkpatrick/73942f26c21dbf6e572b31dcd070d357fc91b356)
