#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cassert>
#include <glog/logging.h>
#include <gflags/gflags.h>
#include "../include/graph.h"
#include "../include/shortest_path.h"
#include "../include/minimum_spanning_tree.h"

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

int main(int argc, char* argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = 1;

    Graph * g = generate_graph_from_file("test/SampleTestData_mst_data.txt");
    KruskalMST mst = KruskalMST(*g);
    double cost = mst.cost();
    std::vector<Edge> mstree = mst.tree();
    LOG(INFO) << "minimum spanning tree cost is " << cost;
    LOG(INFO) << "minimum spanning tree edge num is " << mstree.size();

    SafeDelete(g);
    return 0;
}
