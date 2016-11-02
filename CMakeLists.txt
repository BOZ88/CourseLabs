cmake_minimum_required(VERSION 3.6)
project(FailureDetectorCloudComputingI)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Application.h Application.cpp EmulNet.cpp EmulNet.h Grader.sh Log.cpp Log.h
        Makefile Member.cpp Member.cpp MP1Node.cpp MP1Node.h Params.cpp Params.h Queue.h run.sh
        mp1_specifications.pdf stdincludes.h submit.py testcases)

add_executable(Application ${SOURCE_FILES})
