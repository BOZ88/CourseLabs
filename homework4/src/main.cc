#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cassert>
#include <glog/logging.h>
#include <gflags/gflags.h>
#include "../include/graph.h"

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

int main(int argc, char* argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = 1;

    Graph * g = generate_graph_from_file("src/hex_board.txt");

    SafeDelete(g);
    return 0;
}
