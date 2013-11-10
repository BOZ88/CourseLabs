#include <queue>
#include <cassert>
#include <glog/logging.h>
#include "../include/shortest_path.h"

namespace cpp2c{

std::vector<unsigned int> ShortestPath::vertices(){
    return this->_graph.get_vertices();
}

unsigned int find_nearest_vectex(const std::map<unsigned int, double>& openset){
    assert(openset.size() > 0);
    unsigned int nearest_vertex = openset.begin()->first;
    double cost = openset.begin()->second;
    for(std::map<unsigned int, double>::const_iterator it = openset.begin(); it != openset.end(); ++it){
        if(it->second < cost){
            nearest_vertex = it->first;
            cost = it->second;
        }
    }
    return nearest_vertex;
}

std::vector<unsigned int> gather_path(const std::map<unsigned int, unsigned int>& pre_vertex, unsigned int u, unsigned int w){
    VLOG(VERBOSITY_LOW) << "gather_path from " << u << " to " << w;
    for(std::map<unsigned int, unsigned int>::const_iterator it = pre_vertex.begin(); it != pre_vertex.end(); it++){
        VLOG(VERBOSITY_MID) << "pre_vertex " << it->first << ": " << it->second ;
    }

    std::vector<unsigned int> tmp_path;
    tmp_path.push_back(w);
    unsigned int pv = pre_vertex.at(w);
    while(pv != u){
        tmp_path.insert(tmp_path.begin(), pv);
        pv = pre_vertex.at(pv);
    }
    tmp_path.insert(tmp_path.begin(), u);

    return tmp_path;
}

std::vector<unsigned int> ShortestPath::path(unsigned int u, unsigned int w){
    assert(u < this->_graph.V());
    assert(w < this->_graph.V());

    if(u == w)
        return std::vector<unsigned int>(0);

    std::map<unsigned int, double> openset;
    std::map<unsigned int, double> closeset;
    std::map<unsigned int, unsigned int> pre_vertex;

    unsigned int nearest_vertex = u;
    openset[u] = 0;

    while(openset.size() != 0){
        VLOG(VERBOSITY_MID) << "current nearest_vertex " << nearest_vertex;

        VLOG(VERBOSITY_MID) << "vertex " << nearest_vertex << " add to closeset, remove from openset";
        closeset[nearest_vertex] = this->_graph.get_edge_value(u, nearest_vertex);
        openset.erase(nearest_vertex);

        std::vector<unsigned int> neighbours = this->_graph.neighbours(nearest_vertex);
        for(std::vector<unsigned int>::iterator it = neighbours.begin(); it != neighbours.end(); ++it){
            if(closeset.find(*it) != closeset.end())
                continue;

            double tmp_cost = this->_graph.get_edge_value(u, nearest_vertex) +
                              this->_graph.get_edge_value(nearest_vertex, *it);

            if(this->_graph.adjacent(u, *it)){
                double current_cost = this->_graph.get_edge_value(u, *it);
                VLOG(VERBOSITY_MID) << "vertex " << *it << " tmp cost "
                    << tmp_cost << ",get_edge_value " << current_cost;
                if(tmp_cost <= current_cost){
                    this->_graph.set_edge_value(u, *it, tmp_cost);
                    this->_graph.set_edge_value(*it, u, tmp_cost); //undirect graph
                    pre_vertex[*it] = nearest_vertex;
                }
            } else {
                this->_graph.set_edge_value(u, *it, tmp_cost);
                this->_graph.set_edge_value(*it, u, tmp_cost);
                pre_vertex[*it] = nearest_vertex;
            }

            // add neighbour to openset
            openset[*it] = this->_graph.get_edge_value(u, *it);
        }

        if(openset.size() > 0){
            nearest_vertex = find_nearest_vectex(openset);
            VLOG(VERBOSITY_MID) << "chose nearest_vertex " << nearest_vertex;
        }
    }

    if(closeset.find(w) == closeset.end()){
        LOG(INFO) << "not find a path";
        return std::vector<unsigned int>(0);
    }

   return gather_path(pre_vertex, u, w);
}

bool ShortestPath::path_size(unsigned int u, unsigned int v, double& cost){
    std::vector<unsigned int> p = this->path(u,v);
    if(p.size() != 0){
        cost = this->_graph.get_edge_value(u, v);
        return true;
    }
    cost = 0;
    return false;
}

} //end namespace cpp2c
