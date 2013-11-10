#ifndef _MINIMUM_SPANNING_TREE_
#define _MINIMUM_SPANNING_TREE_

#include <vector>
#include "graph.h"

namespace cpp2c{

class MST
{
public:
    explicit MST(Graph graph) : _isProcessed(false), _graph(graph){}
    double cost();
    std::vector<Edge> tree();
private:
    bool _process();
    bool _isProcessed;
    double _cost;
    std::vector<Edge> _tree;
    Graph _graph;
};

}

#endif // _MINIMUM_SPANNING_TREE_
