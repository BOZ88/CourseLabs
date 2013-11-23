#include <iostream>
#include <glog/logging.h>
#include "../include/hex.h"
#include "../include/graph.h"
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
    for(size_t i = 0; i < HEX_BOARD_SIZE; ++i){
        vector<string> tmp_hexagon;
        for(size_t j = 0; j < HEX_BOARD_SIZE; ++j){
            tmp_hexagon.push_back(".");
        }
        this->_hexagon.push_back(tmp_hexagon);
    }
    return true;
}

void HEX::release(){
    SafeDelete(this->_graph);
}

void HEX::userChooseColor(){
    string color;
    while(color != "b" && color != "r"){
        cout << "Choose color please, BLUE('b') or RED('r'), "
             << "and BLUE going first." << endl;
        cin >> color;
    }
    if(color == "b")
        this->_userColor = "b";
    else
        this->_userColor = "r";
}

void HEX::show_board(){
    string line = "";
    for(size_t i = 0; i < HEX_BOARD_SIZE; ++i){
        line = "";
        for(size_t j = 0; j < HEX_BOARD_SIZE; ++j){
            if(j != HEX_BOARD_SIZE -1)
                line += this->_hexagon[i][j] + " - ";
            else
                line += this->_hexagon[i][j];
        }
        cout << string(2 * i, ' ') << line << endl;

        line = "";
        if(i != HEX_BOARD_SIZE - 1){
            for(size_t j = 0; j < HEX_BOARD_SIZE; ++j){
                if(j != HEX_BOARD_SIZE -1)
                    line += "\\ / ";
                else
                    line += "\\";
            }
            cout << string((2 * i) + 1, ' ') << line << endl;
        }
    }
}

bool HEX::isValid(const size_t x, const size_t y){
    if(x >= HEX_BOARD_SIZE || y >= HEX_BOARD_SIZE)
        return false;
    if(this->_hexagon.at(x).at(y) != ".")
        return false;
    return true;
}

void HEX::play(const size_t x, const size_t y){
    if(!isValid(x, y))
        return;
    this->_hexagon[x][y] = this->_userColor;
    this->show_board();
}

void HEX::start(){
    if(! this->init()){
        return;
    }

    greeting();
    this->userChooseColor();
    this->show_board();

    while(true){

        cout << "input 2 number (example: 12 34) to put a chessman, "
             << "'q' to quit:" << endl;
        string num1;
        string num2;

        cin >> num1;
        if(!isDigits(num1))
            continue;
        cin >> num2;
        if(!isDigits(num2))
            continue;

        if(num1 == "q")
            break;
        this->play(static_cast<size_t>(atoi(num1.c_str())),
                static_cast<size_t>(atoi(num2.c_str())));
    }

    this->release();
}

} // end namespace cpp2c
