// Program output:
//   density 0.2, vertex 50, edge range [1,10], average shortest cost is 7.22301
//   density 0.4, vertex 50, edge range [1,10], average shortest cost is 5.33753
//
// How I implemented the simulation:
//   * I choose connectivity matrices to implement Graph ADT.
//   * The interfaces of ShortestPath are based on the HW2 web page.
//   * I'm not familier with poriority queue, so I simply use loop to chose 
//     a nearest neigbhour vertex in dijstra`s algorithmn, I know is not efficient enough.
//   * Most of the code is self explained. There are necessary documentation on those
//     not obvously understandable code.
//   * The code below is a version for handout, There is another version with more log infomation 
//     on my github repo(https://github.com/doublelduob/cpp2cprogrammers_coursera_homework)
//     which has more log output use glog library.


#include <iostream>
#include <vector>
#include <map>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <cassert>

class Edge{
public:
    Edge() : exists(false), value(0) {}
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

class ShortestPath
{
public:
    ShortestPath(Graph& graph) : _graph(graph){}
    ~ShortestPath(){}

    //list of vertices in graph
    std::vector<unsigned int> vertices();

    //find shortest path between u-w and returns the sequence of vertices
    std::vector<unsigned int> path(int u, int w);

    //return the path cost associated with the shortest path
    bool path_size(unsigned int u,unsigned int w, double& cost);
private:
    Graph _graph;
};

Graph* random_generate_graph(int num_of_vertex, double density, double edge_range_min, double edge_range_max){

    assert(num_of_vertex > 0);
    assert(density > 0);
    assert(density < 1);
    assert(edge_range_min >= 0);
    assert(edge_range_max > edge_range_min);

    Graph *graph = new Graph(num_of_vertex);
    srand(time(NULL));

    // from each vetex to the others
    for(int i = 0; i < num_of_vertex; ++i){
        for(int j = 0; j <= i; ++j){
            if(i == j){
                graph->add_edge(i, j, 0);
                continue;
            }

            double probability = static_cast<double>(rand()) / RAND_MAX;
            if(probability > density)
                continue;

            // get a new random probability 
            probability = static_cast<double>(rand()) / RAND_MAX;
            double cost = probability * (edge_range_max - edge_range_min) + edge_range_min;

            graph->add_edge(i, j, cost);
            graph->add_edge(j, i, cost); // undirected graph
        }
    }
    return graph;
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
    const Edge *e;
    e = &this->_cm.at(x).at(y); // if out of range, just stop
    if(true == e->exists){
        return true;
    }
    return false;
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

    this->_cm.at(x).at(y).exists = true;  // if out of range, just stop
    this->_cm.at(x).at(y).value = cost;

    this->_num_of_edge ++;
    return true;
}

bool Graph::delete_edge(unsigned int x, unsigned int y){
    if( false == this->adjacent(x, y)){
        return false;
    }

    this->_cm.at(x).at(y).exists = false;    // if out of range, just stop
    this->_cm.at(x).at(y).value = 0;
    this->_num_of_edge --;

    return true;
}

double Graph::get_edge_value(unsigned int x, unsigned int y) const{
    const Edge *e = &this->_cm.at(x).at(y);
    assert(e->exists);
    return e->value;
}

bool Graph::set_edge_value(unsigned int x, unsigned int y, double v){
    this->_cm.at(x).at(y).exists = true;  // if out of range, just stop
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

std::vector<unsigned int> ShortestPath::vertices(){
    return this->_graph.get_vertices();
}

inline int find_nearest_vectex(const std::map<int, double>& openset){
    assert(openset.size() > 0);
    int nearest_vertex = openset.begin()->first;
    double cost = openset.begin()->second;
    for(std::map<int, double>::const_iterator it = openset.begin(); it != openset.end(); ++it){
        if(it->second < cost){
            nearest_vertex = it->first;
            cost = it->second;
        }
    }
    return nearest_vertex;
}

// sub function of ShortestPath::path, each vertext only track its pre vertex,
// trace back and store togather.
inline std::vector<unsigned int> gather_path(const std::map<int, int>& pre_vertex, int u, int w){
    std::vector<unsigned int> tmp_path;
    tmp_path.push_back(w);
    int pv = pre_vertex.at(w);
    while(pv != u){
        tmp_path.insert(tmp_path.begin(), pv);
        pv = pre_vertex.at(pv);
    }
    tmp_path.insert(tmp_path.begin(), u);

    return tmp_path;
}

std::vector<unsigned int> ShortestPath::path(int u, int w){
    assert(u < this->_graph.V());
    assert(w < this->_graph.V());

    if(u == w)
        return std::vector<unsigned int>(0);

    std::map<int, double> openset;
    std::map<int, double> closeset;
    std::map<int, int> pre_vertex;

    int nearest_vertex = u;
    openset[u] = 0;

    while(openset.size() != 0){
        closeset[nearest_vertex] = this->_graph.get_edge_value(u, nearest_vertex);
        openset.erase(nearest_vertex);

        std::vector<unsigned int> neighbours = this->_graph.neighbours(nearest_vertex);
        for(std::vector<unsigned int>::iterator it = neighbours.begin(); it != neighbours.end(); ++it){

            // neighbour vertex in closeset, should not be processed.
            if(closeset.find(*it) != closeset.end())
                continue;

            double tmp_cost = this->_graph.get_edge_value(u, nearest_vertex) +
                              this->_graph.get_edge_value(nearest_vertex, *it);

            // if source vertex can reach a neighbour, compare for path relaxiation
            // else set the edge
            if(this->_graph.adjacent(u, *it)){
                double current_cost = this->_graph.get_edge_value(u, *it);
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
        }
    }

    if(closeset.find(w) == closeset.end()){
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

template<typename T> void SafeDelete(T*& p){
    if(NULL != p){
        delete p;
        p = NULL;
    }
}

double compute_average_path_len(ShortestPath& s, unsigned int vertex_num){
    double path_cost_sum = 0;
    double tmp_path_cost = 0;
    int effective_path_num = 0;

    for(unsigned int i = 1; i < vertex_num; ++i){
        if( true == s.path_size((unsigned int)0, i, tmp_path_cost)){
            effective_path_num ++;
            path_cost_sum += tmp_path_cost;
        }
    }
    double average_shortest_cost = path_cost_sum / effective_path_num;
    return average_shortest_cost;
}

int main(int argc, char* argv[])
{
    unsigned int vertex_num = 50;
    double density1 = 0.2;
    double density2 = 0.4;
    double edge_range_min = 1.0;
    double edge_range_max = 10.0;

    Graph* g1 = random_generate_graph(vertex_num, density1, edge_range_min, edge_range_max);
    ShortestPath s1 = ShortestPath(*g1);
    double average_shortest_cost1 = compute_average_path_len(s1, vertex_num);
    std::cout << "density 0.2, vertex 50, edge range [1,10], average shortest cost is "
              << average_shortest_cost1 << std::endl;
    SafeDelete(g1);

    Graph* g2 = random_generate_graph(vertex_num, density2, edge_range_min, edge_range_max);
    ShortestPath s2 = ShortestPath(*g2);
    double average_shortest_cost2 = compute_average_path_len(s2, vertex_num);
    std::cout << "density 0.4, vertex 50, edge range [1,10], average shortest cost is "
              << average_shortest_cost2 << std::endl;
    SafeDelete(g2);

    return 0;
}

