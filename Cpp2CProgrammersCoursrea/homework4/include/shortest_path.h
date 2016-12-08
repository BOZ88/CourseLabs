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
    //list of vertices in graph
    std::vector<unsigned int> vertices();
    //find shortest path between u-w and returns the sequence of vertices
    std::vector<unsigned int> path(unsigned int u, unsigned int w);
    //return the path cost associated with the shortest path
    bool path_size(unsigned int u,unsigned int w, double& cost);
private:
    Graph _graph;

};

} // end namespace cpp2c

#endif //_SHORTEST_PATH_ALGO_
