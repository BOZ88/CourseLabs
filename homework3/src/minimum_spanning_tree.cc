// totally wrong, need rework
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <cassert>
#include <glog/logging.h>
#include "../include/minimum_spanning_tree.h"

using std::vector;
using std::priority_queue;
using std::map;
using std::set;

namespace cpp2c{

struct EdgeCmp{
    bool operator() (const Edge& a, const Edge& b){
        return a.value > b.value;
    }
};

double KruskalMST::cost(){
    if(this->_isProcessed == false){
        assert(true == this->_process());
    }
    return this->_cost;
}

vector<Edge> KruskalMST::tree(){
    if(this->_isProcessed == false){
        assert(true == this->_process());
    }
    return this->_tree;
}

static void init_priority_queue(
        priority_queue<Edge, vector<Edge>, EdgeCmp>& pq,
        Graph& graph){
    unsigned int num_of_vertex = graph.V();

    for(unsigned int i = 0; i < num_of_vertex; ++i){
        for(unsigned int j = 0; j < num_of_vertex; ++j){
            if(graph.adjacent(i, j)){
                double cost = graph.get_edge_value(i, j);
                Edge e = Edge(i, j, cost);
                pq.push(e);
            }
        }
    }
}

static bool get_miminum_edge(
        priority_queue<Edge, vector<Edge>,EdgeCmp>& pq, Edge& e){
    if(pq.empty()){
        return false;
    }

    e = pq.top();
    pq.pop();
    return true;
}

class UnionFind
{
    vector<unsigned int> _parent;
public:
    explicit UnionFind(unsigned int num){
        this->_parent = vector<unsigned int>(num);
        for(unsigned int i = 0; i < num; ++i)
            this->make_set(i);
    }
    void make_set(unsigned int x){
        this->_parent[x] = x;
    }
    unsigned int find(unsigned int x){
        while(x != _parent[x]){
            x = _parent[x];
        }
        return x;
    }
    void do_union(unsigned int x, unsigned int y){
        int root_x = this->find(x);
        int root_y = this->find(y);
        this->_parent[root_y] = root_x;
    }
};

bool KruskalMST::_process(){
    unsigned int num_of_vertex = this->_graph.V();
    priority_queue<Edge, vector<Edge>, EdgeCmp > pq;
    vector<bool> selected_vertex = vector<bool>(num_of_vertex, false);
    UnionFind uf(num_of_vertex);
    vector<Edge> tree;
    Edge tmp_edge;

    init_priority_queue(pq, this->_graph);

    while(get_miminum_edge(pq, tmp_edge)){
        VLOG(VERBOSITY_MID) << "root " << tmp_edge.from << "is "<< uf.find(tmp_edge.from)
                            << " root " << tmp_edge.to << " is " << uf.find(tmp_edge.to)
                            << " cost is " << tmp_edge.value;
        if(uf.find(tmp_edge.from) != uf.find(tmp_edge.to)){
            uf.do_union(tmp_edge.from, tmp_edge.to);
            this->_tree.push_back(tmp_edge);
            this->_cost += tmp_edge.value;
        }
    }

    this->_isProcessed = true;
    return true;
}

} // end namespace
