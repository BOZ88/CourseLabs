#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cassert>
#include <glog/logging.h>
#include <gflags/gflags.h>
#include "../include/graph.h"
#include "../include/shortest_path.h"

using namespace cpp2c;

template<typename T> void SafeDelete(T*& p){
    if(NULL != p){
        VLOG(VERBOSITY_MID) << "pointer " << p
            << " is not NULL, delete and set NULL";
        delete p;
        p = NULL;
    }
}

bool is_digits(const std::string &str){
    return str.find_first_not_of("0123456789") == std::string::npos;
}

void print_test_matrics(const std::string& filename){
    std::ifstream fin(filename.c_str());
    std::string line;
    while(getline(fin,line))
        LOG(INFO) << line;
    fin.close();
}

bool create_test_graph(Graph **g, const std::string& filename){
    assert(g != NULL);
    assert(filename != "");
    VLOG(VERBOSITY_LOW) << "create_test_graph( " << g << ", " << filename << ")";

    print_test_matrics(filename);

    std::string tmp;
    std::ifstream fin(filename.c_str());
    if(fin.is_open()){
        unsigned int n;
        fin >> n;
        *g = new Graph(n);
        for(unsigned int i = 0; i < n; ++i){
            for(unsigned int j = 0; j < n; ++j){
                fin >> tmp;
                VLOG(VERBOSITY_MID) << "[" << i << "][" << j << "]" << tmp;
                if(is_digits(tmp)){
                    (*g)->add_edge(i,j,atof(tmp.c_str()));
                }
            }
        }
        fin.close();
        return true;
    }
    fin.close();
    return false;
}

int main(int argc, char* argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = 1;

    //Graph* g = random_generate_graph((unsigned int)50, 0.1, 1.0, 10.0);
    Graph * g;
    create_test_graph(&g, "test/test1.txt");

    ShortestPath s = ShortestPath(*g);
    std::vector<unsigned int> vertices = s.vertices();

    //double path_cost_sum = 0;
    //int effective_path_num = 0;
    //double tmp_path_cost = 0;
    //for(unsigned int i = 1; i < 50; ++i){
        //if( true == s.path_size((unsigned int)0, i, tmp_path_cost)){
            //VLOG(VERBOSITY_MID) << "shortest path 0 -> " 
                                //<< i << " is " << tmp_path_cost;
            //effective_path_num ++;
            //path_cost_sum += tmp_path_cost;
        //}
    //}
    //double average_shortest_cost = path_cost_sum / effective_path_num;
    //std::cout << "average_shortest_cost is " << average_shortest_cost << std::endl;

    std::vector<unsigned int> path = s.path(0,7);

    LOG(INFO) << "path.size " << path.size();
    for(unsigned int i = 0; i < path.size(); ++i){
        LOG(INFO) << "shortestpath " << path.at(i);
    }

    SafeDelete(g);
    return 0;
}
