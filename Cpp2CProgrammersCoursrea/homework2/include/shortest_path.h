#ifndef _SHORTEST_PATH_ALGO_
#define _SHORTEST_PATH_ALGO_

#include <map>
#include <vector>
#include "graph.h"

namespace cpp2c{

class ShortestPath
{
public:
    ShortestPath(Graph& graph) : _graph(graph){}
    ~ShortestPath(){}
    std::vector<unsigned int> vertices(); //list of vertices in graph
    std::vector<unsigned int> path(unsigned int u, unsigned int w); //find shortest path between u-w and returns the sequence of vertices
    bool path_size(unsigned int u,unsigned int w, double& cost); //return the path cost associated with the shortest path
private:
    Graph _graph;

};

} // end namespace cpp2c

#endif //_SHORTEST_PATH_ALGO_
