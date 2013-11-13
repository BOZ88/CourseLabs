#ifndef _MINIMUM_SPANNING_TREE_
#define _MINIMUM_SPANNING_TREE_

#include <vector>
#include "graph.h"

namespace cpp2c{

class MST
{
public:
    virtual double cost() = 0;
    virtual std::vector<Edge> tree() = 0;
};

class KruskalMST : public MST
{
public:
    explicit KruskalMST(Graph graph) : _isProcessed(false), _cost(0), _graph(graph){}
    double cost();
    std::vector<Edge> tree();
private:
    bool _process();
    bool _isProcessed;
    double _cost;
    std::vector<Edge> _tree;
    Graph _graph;
};

class PrimMST : public MST
{
public:
    explicit PrimMST(Graph graph);
    double cost();
    std::vector<Edge> tree();
};

}

#endif // _MINIMUM_SPANNING_TREE_
