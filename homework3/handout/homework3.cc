// 1. I choose B.Kruskal algorithmn to solve MST problem
// 2. The main function first read a test file to generate a graph.
//    then get MST cost, and tree.
// 3. When implemete the B.Kruskal MST, I do not put the main task in constructor 
//    or a function "user" should invoke manmually. I think not process until "user" really 
//    get the minimum cost or the tree is just the right time.
//    

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <cassert>


class Edge{
public:
    Edge() : exists(false), value(0) {}
    Edge(unsigned int from, unsigned int to, double value): from(from), to(to), value(value){}
    unsigned int from;
    unsigned int to;
    bool exists;
    double value;
};

// Use connectivity matrices
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


Graph* generate_graph_from_file(const std::string& filename);

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

        }
    }

    if(num_of_selected_vertex < num_of_vertex){
        return false;
    }

    this->_isProcessed = true;
    this->_cost = cost;
    this->_tree = tree;
    return true;
}

Graph* generate_graph_from_file(const std::string& filename){
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
    }
    return NULL;
}

Graph::Graph(unsigned int num_of_vertex){
    this->_num_of_vertex = num_of_vertex;
    this->_num_of_edge = 0;
    std::vector<Edge> tmp(this->_num_of_vertex);
    this->_cm = std::vector<std::vector<Edge> >(this->_num_of_vertex, tmp);
}

unsigned int Graph::V() const{
    return this->_num_of_vertex;
}

unsigned int Graph::E() const{
    return this->_num_of_edge;
}

bool Graph::adjacent(unsigned int x, unsigned int y) const{
    const Edge *e = &this->_cm.at(x).at(y);
    if(true == e->exists){
        return true;
    } else {
        return false;
    }
}

const std::vector<unsigned int> Graph::neighbours(unsigned int x) const{
    std::vector<unsigned int> neighbours_p;
    for(unsigned int i = 0; i < this->_num_of_vertex; ++i){
        if( true == this->adjacent(x, i)){
            neighbours_p.push_back(i);
        }
    }
    return neighbours_p;
}

bool Graph::add_edge(unsigned int x, unsigned int y, double cost){

    if( true == this->adjacent(x, y)){
        return false;
    }

    this->_cm.at(x).at(y).exists = true;
    this->_cm.at(x).at(y).value = cost;
    this->_cm.at(x).at(y).from = x;
    this->_cm.at(x).at(y).to = y;

    this->_num_of_edge ++;
    return true;
}

bool Graph::delete_edge(unsigned int x, unsigned int y){

    if( false == this->adjacent(x, y)){
        return false;
    }
    this->_cm.at(x).at(y).exists = false;
    this->_cm.at(x).at(y).value = 0;
    this->_cm.at(x).at(y).from = 0;
    this->_cm.at(x).at(y).to = 0;
    this->_num_of_edge --;
    return true;
}

double Graph::get_edge_value(unsigned int x, unsigned int y) const{
    const Edge *e = &this->_cm.at(x).at(y);
    return e->value;
}

bool Graph::set_edge_value(unsigned int x, unsigned int y, double v){
    this->_cm.at(x).at(y).exists = true;
    this->_cm.at(x).at(y).value = v;

    return true;
}

std::vector<unsigned int> Graph::get_vertices(){
    std::vector<unsigned int> t;
    for(unsigned int i=0; i <= this->V(); ++i){
        t.push_back(i);
    }
    return t;
}

template<typename T> void SafeDelete(T*& p){
    if(NULL != p){
        delete p;
        p = NULL;
    }
}

int main(int argc, char* argv[])
{
    Graph * g = generate_graph_from_file("mst_data.txt");
    if(NULL == g){
        std::cout << "Can not find mst_data.txt in current directory" << std::endl;
        return -1;
    }

    MST mst = MST(*g);
    double cost = mst.cost();
    std::vector<Edge> mstree = mst.tree();
    std::cout << "minimum spanning tree cost is :" << cost << std::endl;

    std::cout << "minimum spanning tree edge is :" << std::endl;
    for(unsigned long i = 0; i < mstree.size(); ++i){
        std::cout << " edge from " << mstree.at(i).from 
                  << " to " << mstree.at(i).to << " cost " 
                  << mstree.at(i).value << std::endl;
    }

    SafeDelete(g);
    return 0;
}

