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
#include <set>
#include <queue>
#include <map>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <cassert>

using std::vector;
using std::map;
using std::set;
using std::priority_queue;

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
    virtual double cost() = 0;
    virtual std::vector<Edge> tree() = 0;
};

class KruskalMST : public MST
{
public:
    explicit KruskalMST(Graph graph) : _isProcessed(false), _cost(0), _graph(graph) {}

    // get total cost of the minimum spanning tree
    double cost();

    // get total edges of the minimum spanning tree
    std::vector<Edge> tree();
private:
    bool _process();  // meta functions to do the real work
    bool _isProcessed; // check the MST is processed or not
    double _cost;
    std::vector<Edge> _tree;
    Graph _graph;
};

Graph* generate_graph_from_file(const std::string& filename);

// used in priority queue to compare to edges
struct EdgeCmp{
    bool operator() (const Edge& a, const Edge& b){
        return a.value > b.value;
    }
};

double KruskalMST::cost(){
    // put the process function here, making the work done when
    // it is really needed. and "user" do not need call process
    // function explicitly.
    if(this->_isProcessed == false){
        assert(true == this->_process());
    }
    return this->_cost;
}

// put all edges in priority queue
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

vector<Edge> KruskalMST::tree(){
    if(this->_isProcessed == false){
        assert(true == this->_process());
    }
    return this->_tree;
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
        if(uf.find(tmp_edge.from) != uf.find(tmp_edge.to)){
            uf.do_union(tmp_edge.from, tmp_edge.to);
            this->_tree.push_back(tmp_edge);
            this->_cost += tmp_edge.value;
        }
    }

    this->_isProcessed = true;
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

            // ignore the empty line
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
        std::cout << "Can not open mst_data.txt in current directory" << std::endl;
        return -1;
    }

    KruskalMST mst = KruskalMST(*g);
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

