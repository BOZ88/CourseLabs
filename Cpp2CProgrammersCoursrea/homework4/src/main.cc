#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <glog/logging.h>
#include <gflags/gflags.h>
#include "../include/util.h"
#include "../include/graph.h"
#include "../include/hex.h"

using namespace cpp2c;


int main(int argc, char* argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = 1;

    HEX().start();

    return 0;
}
