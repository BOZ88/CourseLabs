#ifndef _HEX_H_
#define _HEX_H_

#include <string>
#include <vector>
#include "../include/graph.h"

namespace cpp2c{

const int HEX_BOARD_SIZE = 7;

class HEX
{
public:
    HEX(){}
    ~HEX(){}
    void start();
private:
    bool init();
    void release();
    void userChooseColor();
    void play(const size_t, const size_t);
    void show_board();
    bool isValid(size_t, size_t);

    Graph *_graph;
    std::string _userColor;
    std::vector<std::vector<std::string> > _hexagon;
};

} // end namespace cpp2c

#endif
