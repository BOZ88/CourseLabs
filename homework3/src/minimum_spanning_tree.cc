#include <vector>
#include <queue>
#include <cassert>
#include <glog/logging.h>
#include "../include/minimum_spanning_tree.h"

namespace cpp2c{

struct EdgeCmp{
    bool operator() (const Edge& a, const Edge& b){
        return a.value > b.value;
    }
};

double MST::cost(){
    if(this->_isProcessed == false){
        assert(true == this->_process());
    }
    return this->_cost;
}

std::vector<Edge> MST::tree(){
    if(this->_isProcessed == false){
        assert(true == this->_process());
    }
    return this->_tree;
}

static void init_priority_queue(std::priority_queue<Edge, std::vector<Edge>, EdgeCmp>& pq, Graph& graph){
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

static bool get_miminum_edge(std::priority_queue<Edge, std::vector<Edge>, EdgeCmp>& pq, Edge& e){
    if(pq.empty()){
        return false;
    }

    e = pq.top();
    pq.pop();
    return true;
}

bool MST::_process(){
    std::priority_queue<Edge, std::vector<Edge>, EdgeCmp > pq;
    std::vector<Edge> tree;
    unsigned int num_of_vertex = this->_graph.V();
    std::vector<bool> selected_vertex = std::vector<bool>(num_of_vertex, false);
    unsigned int num_of_selected_vertex = 0;
    double cost = 0;

    init_priority_queue(pq, this->_graph);

    while(num_of_selected_vertex < num_of_vertex){
        Edge e;
        if(false == get_miminum_edge(pq, e)){
            LOG(INFO) << "There is no edge in priority_queue";
            break;
        }
        if(false == selected_vertex.at(e.from) || 
           false == selected_vertex.at(e.to)){

            if(false == selected_vertex.at(e.from) &&
               false == selected_vertex.at(e.to)){
                ++num_of_selected_vertex;
            }

            tree.push_back(e);
            cost += e.value;
            selected_vertex.at(e.from) = true;
            selected_vertex.at(e.to) = true;
            ++num_of_selected_vertex;

            VLOG(VERBOSITY_MID) << "select " << e.from << ", " << e.to 
                  << " cost " << e.value << " num_of_selected_vertex " 
                  << num_of_selected_vertex;
        }
    }

    if(num_of_selected_vertex < num_of_vertex){
        LOG(WARNING) << "graph is not a connected graph";
        return false;
    }

    this->_isProcessed = true;
    this->_cost = cost;
    this->_tree = tree;
    return true;
}

} // end namespace
