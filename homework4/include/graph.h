#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>
#include <string>

namespace cpp2c{

const int VERBOSITY_LOW = 2;
const int VERBOSITY_MID = 5;

class Edge{
public:
    Edge() : exists(false), value(0) {}
    Edge(unsigned int from, unsigned int to, double value): from(from), to(to), value(value){}
    unsigned int from;
    unsigned int to;
    bool exists;
    double value;
};


class Graph
{
public:
    explicit Graph(unsigned int num_of_vertex);
    Graph() : _num_of_vertex(0), _num_of_edge(0){}
    ~Graph() {};

    unsigned int V() const; // returns the number of vertices in the graph
    unsigned int E() const; // returns the number of edges in the graph

    // tests whether there is an edge from node x to node y
    bool adjacent(unsigned int x, unsigned int y) const;

    // lists all nodes y such that there is an edge from x to y
    const std::vector<unsigned int> neighbours(unsigned int x) const;

    // adds to graph the edge from x to y, if it is not there
    bool add_edge(unsigned int x, unsigned int y, double cost);

    // removes the edge from x to y, if ti is there.
    bool delete_edge(unsigned int x, unsigned int y);

    // returns the vallue associated with the node x.
    // int get_node_value(unsigned int x) const;

    // sets the value associated with the node x to a
    // bool set_node_value(unsigned int x);

    // returns the value associated to the edge (x,y)
    double get_edge_value(unsigned int x, unsigned int y) const;

    // sets the value associated to the edge (x,y) to v.
    bool set_edge_value(unsigned int x, unsigned int y, double v);

    std::vector<unsigned int> get_vertices();

private:
    std::vector<std::vector<Edge> > _cm; // connectivity matrices
    unsigned int _num_of_vertex;
    unsigned int _num_of_edge;
};

Graph* random_generate_graph(unsigned int num_of_vertex, double density, double edge_range_min, double edge_range_max);
Graph* generate_graph_from_file(const std::string& filename);

} // endnamespace
#endif
