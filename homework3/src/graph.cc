#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <limits>
#include <stdexcept>
#include <glog/logging.h>
#include "../include/graph.h"


namespace cpp2c{

Graph* random_generate_graph(unsigned int num_of_vertex, double density, double edge_range_min, double edge_range_max){
    VLOG(VERBOSITY_LOW) << "random_generate_graph(num_of_vertex " << num_of_vertex << ", "
        << "density " << density << ", edge_range_min " << edge_range_min 
        << ", edge_range_max " << edge_range_max << ")";

    if( density < 0 || edge_range_min < 0 || edge_range_max < edge_range_min){
        LOG(WARNING) << "invalid parameter, return NULL";
        return NULL;
    }

    Graph *graph = new Graph(num_of_vertex);
    srand(static_cast<unsigned int>(time(NULL)));

    for(unsigned int i = 0; i < num_of_vertex; ++i){
        for(unsigned int j = 0; j <= i; ++j){
            if(i == j){
                graph->add_edge(i, j, 0);
                continue;
            }
            double probability = static_cast<double>(rand()) / RAND_MAX;

            if(probability > density)
                continue;
            VLOG(VERBOSITY_MID) << "probability " << probability << " < density " << density;

            probability = static_cast<double>(rand()) / RAND_MAX;
            double cost = probability * (edge_range_max - edge_range_min) + edge_range_min;
            graph->add_edge(i, j, cost);
            graph->add_edge(j, i, cost); // undirected graph
        }
    }
    return graph;
}

Graph* generate_graph_from_file(const std::string& filename){
    VLOG(VERBOSITY_LOW) << "generate_graph_from_file(filename " << filename << ")";
    assert(filename != "");
    unsigned int num_of_vertex = 0;
    Graph * graph;
    std::ifstream fin(filename.c_str());
    if(fin.is_open()){
        fin >> num_of_vertex;
        graph = new Graph(num_of_vertex);

        while(!fin.eof()){
            std::string line = "";
            std::getline(fin, line);
            if(line.empty())
                continue;
            std::istringstream iss(line);

            unsigned int i = 0;
            unsigned int j = 0;
            double cost = 0;

            iss >> i >> j >> cost;
            graph->add_edge(i, j, cost);
        }
        fin.close();
        return graph;
    } else {
        LOG(WARNING) << "can not open file " << filename;
    }
    return NULL;
}

Graph::Graph(unsigned int num_of_vertex){
    VLOG(VERBOSITY_LOW) << "Graph::Graph(" << num_of_vertex << ")";
    this->_num_of_vertex = num_of_vertex;
    this->_num_of_edge = 0;
    std::vector<Edge> tmp(this->_num_of_vertex);
    this->_cm = std::vector<std::vector<Edge> >(this->_num_of_vertex, tmp);
}

unsigned int Graph::V() const{
    VLOG(VERBOSITY_LOW) << "Graph::V(), return " << this->_num_of_vertex;
    return this->_num_of_vertex;
}

unsigned int Graph::E() const{
    VLOG(VERBOSITY_LOW) << "Graph::E(), return " << this->_num_of_edge;
    return this->_num_of_edge;
}

bool Graph::adjacent(unsigned int x, unsigned int y) const{
    VLOG(VERBOSITY_LOW) << "Graph::adjacent(" << x << ", " << y << ")";

    const Edge *e;
    try{
        e = &this->_cm.at(x).at(y);
    }catch (const std::out_of_range){
        LOG(WARNING) << "Graph::adjacent(" << x << ", " 
            << y << ") out of range";
    }
    if(true == e->exists){
        VLOG(VERBOSITY_MID) << x <<  ", " << y << "is adjacent";
        return true;
    } else {
        VLOG(VERBOSITY_MID) << x <<  ", " << y << " is not adjacent";
        return false;
    }
}

const std::vector<unsigned int> Graph::neighbours(unsigned int x) const{
    VLOG(VERBOSITY_LOW) << "Graph::neighbours(" << x << ")"; 

    std::vector<unsigned int> neighbours_p;
    for(unsigned int i = 0; i < this->_num_of_vertex; ++i){
        if( true == this->adjacent(x, i)){
            VLOG(VERBOSITY_MID) << "found neighbour " << i;
            neighbours_p.push_back(i);
        }
    }
    return neighbours_p;
}

bool Graph::add_edge(unsigned int x, unsigned int y, double cost){
    VLOG(VERBOSITY_LOW) << "Graph::add_edge(" << x << ", " << y << ", cost="<< cost << ")";

    if( true == this->adjacent(x, y)){
        LOG(WARNING) << "edge " << x << "," << y << " already exists, return false";
        return false;
    }
    try{
        this->_cm.at(x).at(y).exists = true;
        this->_cm.at(x).at(y).value = cost;
        this->_cm.at(x).at(y).from = x;
        this->_cm.at(x).at(y).to = y;
    }catch (const std::out_of_range){
        LOG(FATAL) <<"add edge" << x << ", " << y << " out of range";
    }

    this->_num_of_edge ++;
    VLOG(VERBOSITY_MID) << "add edge success, current num is " 
        << this->_num_of_edge;
    return true;
}

bool Graph::delete_edge(unsigned int x, unsigned int y){
    VLOG(VERBOSITY_LOW) << "Graph::delete_edge(" << x << ", " << y << ")";

    if( false == this->adjacent(x, y)){
        LOG(WARNING) << "edge not exists, return false";
        return false;
    }
    try{
        this->_cm.at(x).at(y).exists = false;
        this->_cm.at(x).at(y).value = 0;
        this->_cm.at(x).at(y).from = 0;
        this->_cm.at(x).at(y).to = 0;
    }catch (const std::out_of_range){
        LOG(FATAL) << "Graph::delete edge " << x << ", " << y << "out of range";
    }
    this->_num_of_edge --;
    VLOG(VERBOSITY_MID) << "delete edge success, current num is "
        << this->_num_of_edge;
    return true;
}

double Graph::get_edge_value(unsigned int x, unsigned int y) const{
    VLOG(VERBOSITY_LOW) << "Graph::get_edge_value(" << x << ", " << y << ")";

    const Edge *e;
    try{
        e = &this->_cm.at(x).at(y);
    }catch (const std::out_of_range){
        LOG(WARNING) << "Graph::get_edge_value(" 
            << x << ", " << y << ") out of range";
    }
    if(false == e->exists){
        LOG(FATAL) << "edge " << x << ", " << y 
        << " is not reachable currently";
    }
    return e->value;
}

bool Graph::set_edge_value(unsigned int x, unsigned int y, double v){
    VLOG(VERBOSITY_LOW) << "Graph::set_edge_value(" << x << ", " << y << ", " << v << ")";
        // if edge exists
    try{
        this->_cm.at(x).at(y).exists = true;
        this->_cm.at(x).at(y).value = v;
    }catch (const std::out_of_range){
        LOG(WARNING) << "Graph::set_edge_value(" << x 
            << ", " << y << ", " << v << ") out of range";
    }

    VLOG(VERBOSITY_MID) << "set [" << x << "][" << y << "]=" << v;
    return true;
}

std::vector<unsigned int> Graph::get_vertices(){
    std::vector<unsigned int> t;
    for(unsigned int i=0; i <= this->V(); ++i){
        t.push_back(i);
    }
    return t;
}

} //end namespace

