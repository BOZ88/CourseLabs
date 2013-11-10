###Objective

Trees are an important subcategory of graphs. Trees such as binary trees and red-black trees are important data structures used throughout computer science. Again it is hoped that this topic is mostly review for active participants in this class. 

This assignment requires the implementation of a minimum spanning tree (MST) algorithm for a weighted undirected graph (Graph). A spanning tree for a Graph g is a tree that reaches all the nodes of the graph. The cost for such a spanning tree is the sum of all the edge costs (weights) in the spanning tree’s edge set. Over all possible spanning trees, the MST is the one with the least total cost (it may not be unique).  

Two efficient methods will be explained in this week’s Module, namely Prim’s algorithm and Kruskal’s algorithm. Additionally, the assignment makes the following simplifying assumptions about the unweighted Graph:
Graph is connected. This assumption enables the following invariant: MST invariant states that the edge set for a MST must (i) consist of a set of (V-1) edges and (ii) no edge can create a cycle. 
Edge weights may be zero or non-negative. Non-negative edge weights are not a requirement for constructing a MST, but may simplify our implementation. 
Edge weights are NOT necessarily different.


### Notes and Reminders:

Update your Homework 2 graph class to add two more methods. Add a constructor that can read in a graph from a file. The file format will be an initial integer that is the node size of the graph and the further values will be integer triples: (i, j, cost). See Sample Test Data here.

Implement either the Prim or Kruskal Minimum Spanning Tree Algorithm. These algorithms are similar in flavor to Dijkstra’s shortest path algorithm. Links to these algorithms are provided besides being explained in the videos.

http://en.wikipedia.org/wiki/Kruskal's_algorithm
http://en.wikipedia.org/wiki/Prim_algorithm 
http://en.wikipedia.org/wiki/Minimum_spanning_tree 

The output for MST should be the cost and the tree (the edges).  The input will be provided on the website.

More challenging is an extension to this problem discussed in Module 4 as extra credit, however, for the purpose of this Homework, no extra credit will be given. This is to give each edge one of three colors. Consider that a road may be a local street or a highway or a turnpike.  Many route finding algorithms find a least costly path constrained by what type of road to take. We mimic this with colors and ask that you compute MST where the edge colors are a constraint, eg. all edges must be green. You are welcome to try this problem but the peer grading should be based on the simpler problem where there are no colors.