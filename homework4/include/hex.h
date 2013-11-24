#ifndef _HEX_H_
#define _HEX_H_

#include <string>
#include <vector>
#include <set>
#include "../include/graph.h"

namespace cpp2c{

const unsigned int HEX_BOARD_SIZE = 7;

class HEX
{
public:
    HEX():_graph(NULL), _blue_graph(NULL), _red_graph(NULL){}
    ~HEX(){}
    void start();
private:
    bool init();
    void release();
    void userChooseColor();
    void makeAMove(const std::string& color, const unsigned int, const unsigned int);
    void show_board();
    bool isMoveValid(unsigned int, unsigned int);
    bool isWon(const std::string& color);

    Graph *_graph;
    Graph *_blue_graph;
    Graph *_red_graph;
    std::set<unsigned int> _blue_vertices;
    std::set<unsigned int> _red_vertices;
    std::string _userColor;
    std::vector<std::vector<std::string> > _hexagon;
};

} // end namespace cpp2c

#endif
