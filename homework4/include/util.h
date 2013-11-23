#ifndef _UTIL_H_
#define _UTIL_H_

#include <string>
#include <glog/logging.h>

template<typename T> void SafeDelete(T*& p){
    if(NULL != p){
        LOG(INFO) << "pointer " << p
            << " is not NULL, delete and set NULL";
        delete p;
        p = NULL;
    }
}

bool isDigits(const std::string &str);

#endif
