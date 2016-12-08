#include <iostream>
#include <cassert>
#include <glog/logging.h>
#include "../include/hex.h"
#include "../include/graph.h"
#include "../include/shortest_path.h"
#include "../include/util.h"

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::vector;
using std::atoi;

namespace cpp2c{

static inline void greeting(){
    cout << "Welcome to HEX !" << endl;
}

bool HEX::init(){
    this->_graph = generate_graph_from_file("src/hex_board.txt");
    if(this->_graph == NULL)
        return false;

    this->_blue_graph = new Graph(HEX_BOARD_SIZE * HEX_BOARD_SIZE);

    this->_red_graph = new Graph(HEX_BOARD_SIZE * HEX_BOARD_SIZE);

    for(unsigned int i = 0; i < HEX_BOARD_SIZE; ++i){
        vector<string> tmp_hexagon;
        for(unsigned int j = 0; j < HEX_BOARD_SIZE; ++j){
            tmp_hexagon.push_back(".");
        }
        this->_hexagon.push_back(tmp_hexagon);
    }

    return true;
}

void HEX::release(){
    SafeDelete(this->_graph);
    SafeDelete(this->_blue_graph);
    SafeDelete(this->_red_graph);
}

void HEX::userChooseColor(){
    string color = "";
    while(color != "b" && color != "r"){
        cout << "Type 'b' to choose BLUE which going first, or " << endl;
        cout << "Type 'r' to choose RED" << endl;
        cin >> color;
    }
    if(color == "b")
        this->_userColor = "B";
    else
        this->_userColor = "R";
}

void HEX::show_board(){
    string line = "";
    for(unsigned int i = 0; i < HEX_BOARD_SIZE; ++i){
        line = "";
        for(unsigned int j = 0; j < HEX_BOARD_SIZE; ++j){
            if(j != HEX_BOARD_SIZE -1)
                line += this->_hexagon[i][j] + " - ";
            else
                line += this->_hexagon[i][j];
        }
        cout << string(2 * i, ' ') << line << endl;

        line = "";
        if(i != HEX_BOARD_SIZE - 1){
            for(unsigned int j = 0; j < HEX_BOARD_SIZE; ++j){
                if(j != HEX_BOARD_SIZE -1)
                    line += "\\ / ";
                else
                    line += "\\";
            }
            cout << string((2 * i) + 1, ' ') << line << endl;
        }
    }
}

bool HEX::isMoveValid(const unsigned int x, const unsigned int y){
    if(x >= HEX_BOARD_SIZE || y >= HEX_BOARD_SIZE)
        return false;
    if(this->_hexagon.at(x).at(y) != ".")
        return false;
    return true;
}

bool HEX::isWon(const string& color){
    VLOG(VERBOSITY_MID) << "isWon " << color << endl;

    if(color == "B"){
        ShortestPath sp = ShortestPath(*this->_blue_graph);
        for(unsigned int i = 0; i < HEX_BOARD_SIZE; ++i){
            for(unsigned int j = 0; j < HEX_BOARD_SIZE; ++j){
                unsigned int to = HEX_BOARD_SIZE * (HEX_BOARD_SIZE - 1) + j;
                VLOG(VERBOSITY_MID) << "is there a path from " << i << " to " << to << endl;
                if(sp.path(i, to).size() != 0)
                    return true;
            }
        }
    } else {
        ShortestPath sp = ShortestPath(*this->_red_graph);
        for(unsigned int i = 0; i < HEX_BOARD_SIZE; ++i){
            for(unsigned int j = 0; j < HEX_BOARD_SIZE; ++j){
                unsigned int from = i * HEX_BOARD_SIZE;
                unsigned int to = HEX_BOARD_SIZE * j + (HEX_BOARD_SIZE - 1);
                if(sp.path(from, to).size() != 0)
                    return true;
            }
        }
    }
    return false;
}

void HEX::makeAMove(const string& color, const unsigned int x, const unsigned int y){
    LOG(INFO) << "makeAMove " << color << " " << x << " " << y << endl;
    assert(x >= 0);
    assert(y >= 0);

    if(!isMoveValid(x, y))
        return;
    this->_hexagon[x][y] = color;
    unsigned int vertex = x * HEX_BOARD_SIZE + y;

    if(color == "B"){
        this->_blue_vertices.insert(vertex);

        vector<unsigned int> neighbours = this->_graph->neighbours(vertex);
        for(vector<unsigned int>::iterator it = neighbours.begin();
                it != neighbours.end(); ++it){
            if(this->_blue_vertices.find(*it) != this->_blue_vertices.end()){
                this->_blue_graph->add_edge(vertex, *it, 1);
                this->_blue_graph->add_edge(*it, vertex, 1);
            }
        }
    } else {
        this->_red_vertices.insert(vertex);

        vector<unsigned int> neighbours = this->_graph->neighbours(vertex);
        for(vector<unsigned int>::iterator it = neighbours.begin();
                it != neighbours.end(); ++it){
            if(this->_red_vertices.find(*it) != this->_red_vertices.end()){
                this->_red_graph->add_edge(vertex, *it, 1);
                this->_red_graph->add_edge(*it, vertex, 1);
            }
        }

    }
}

void HEX::play(){
     while(true){
        this->show_board();
        cout << "input 2 number (example: 2 3) to make a move, "
             << "<CTRL - C> to quit:" << endl;

        string num1;
        string num2;

        cin >> num1;
        if(!isDigits(num1))
            continue;
        cin >> num2;
        if(!isDigits(num2))
            continue;

        this->makeAMove(this->_userColor,
                static_cast<unsigned int>(atoi(num1.c_str())),
                static_cast<unsigned int>(atoi(num2.c_str())));

        if(this->isWon(this->_userColor)){
            this->show_board();
            break;
        }
    }
}

void HEX::start(){
    greeting();
    while(true){
        if(!this->init()){
            return;
        }

        this->userChooseColor();
        this->play();
        this->release();

        string s = "";
        cout << "You wanna play again? Type n to quit" << endl;
        cin >> s;
        if(s == "n")
            break;
    }
}

} // end namespace cpp2c
