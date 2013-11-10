###Objective

Implement a Monte Carlo simulation that calculates the average shortest path in a graph. The graph can be generated using a pseudo-random number generator to produce edges and their costs. The shortest path algorithm will be Dijkstra’s. 

Follow the style guidelines in the text, proper indention, one-line per statement, good choice of identifiers will figure prominently in grading. 

Peer grading standards will be based on documentation, good style, correctness, and elegance. 

###Reference Abstractions:  
Implementing Dijkstra’s algorithm requires thinking about at least three basic abstractions: Graph (G = (V, E), PriorityQueue, and ShortestPath algorithm.  Additionally, deciding on a scheme for naming vertices (V) is an important first step in implementation design. By convention, vertices are generally mapped onto the set of Integers in the range from 0 : |V| -1.  This provides an effective Key into sequential containers (like ARRAY) to access vertex records in constant time - Θ(1). Also, in models where vertex names are not represented with integers, the use of a symbol table could be used to provide a 1-to-1 mapping to associate V arbitrary names with V integers in the proper range.

A potential partial interface definition for a Graph could be:

	Class Graph
		
	V (G): returns the number of vertices in the graph
	E (G): returns the number of edges in the graph
	adjacent (G, x, y): tests whether there is an edge from node x to node y.
	neighbors (G, x): lists all nodes y such that there is an edge from x to y.
	add (G, x, y): adds to G the edge from x to y, if it is not there.
	delete (G, x, y): removes the edge from x to y, if it is there.
	get_node_value (G, x): returns the value associated with the node x.
	set_node_value( G, x, a): sets the value associated with the node x to a.
	get_edge_value( G, x, y): returns the value associated to the edge (x,y).
	set_edge_value (G, x, y, v): sets the value associated to the edge (x,y) to v.
One important consideration for the Graph class is how to represent the graph as a member ADT. Two basic implementations are generally considered: adjacency list and adjacency matrix depending on the relative edge density. For sparse graphs, the list approach is typically more efficient, but for dense graphs, the matrix approach can be more efficient (reference an Algorithm’s source for space and time analysis). Note in some cases such as add(G, x, y) you may also want to have the edge carry along its cost. Another approach could be to use (x, y) to index a cost stored in an associated array or map.

The value of the PriorityQueue is to always have access to the vertex with the next shortest link in the shortest path calculation at the top of the queue. A typically implementation is a minHeap:

	Class PriorityQueue
	
	chgPrioirity(PQ, priority): changes the priority (node value) of queue element.
	minPrioirty(PQ): removes the top element of the queue.
	contains(PQ, queue_element): does the queue contain queue_element.
	Insert(PQ, queue_element): insert queue_element into queue
	top(PQ):returns the top element of the queue.
	size(PQ): return the number of queue_elements.
	Finally, the class: ShortestPathAlgo - implements the mechanics of Dijkstra’s algorithm. Besides having member fields (has a relationship) of Graph and Priority Queue, an additional ADT maybe required to maintain the parent relationship of the shortest path.

	Class ShortestPath
	
	vertices(List): list of vertices in G(V,E).
	path(u, w): find shortest path between u-w and returns the sequence of vertices representing shorest path u-v1-v2-…-vn-w.
	path_size(u, w): return the path cost associated with the shortest path.
	The class implementing your Monte Carlo simulation is the workflow manager for this assignment, butother helper classes may be necessary depending on your particular implementation

### Notes and Reminders:

+ Write an appropriate set of constructors for each of your classes ensuring proper initialization – especially think about the process for declaring and initializing a graph. 
+ In this implementation, assume that an edge will have a positive cost function like distance (no negative edge cost). 
+ Assume the graph edges (E)  are undirected.
+ Ensure that your ADTs support a graph of at least size 50.
+ The random graph procedure should have edge density as a parameter and distance range as a parameter.
+ Random graph generator should generate a sufficient set of edges to satisfy the edge density parameter, and each edge should be assigned a randomly generated cost based on the distance range parameter.
+ So a graph whose density is 0.1 would have 10% of its edges picked at random and its edge distance would be selected at random from the distance range. 
+ Compute for a set of randomly generated graphs an average shortest path.


URL’s
http://en.wikipedia.org/wiki/Dijkstra's_algorithm
The time allotted for this problem is two weeks. Beginners should spend the first week creating a graph class with appropriate constructors. The second week can be used to implement and test Dijkstra’s algorithm.