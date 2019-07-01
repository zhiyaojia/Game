#ifndef _BOARD_H
#define _BOARD_H
#include <string>
#include <vector>
#include <iostream>
#include "object.h"

class Board
{
public:
    Board(std::vector<std::string> menu);
    Board(const Board& b);
    ~Board();

    void print(Loc loc);
    void board_change(int row, int col, char ch);
    void change();
    void change_player(std::string role);

    int height() const;
    int width() const;
    int marginx() const;

    char loc(int row, int col) const;

    bool random() const;

    std::string role_, guide, scene;

    bool guide_;

private:
    bool notedge(int row, int col);
    bool random_;
    bool shadow_;
    bool **random_arr;

    char **arr;

    int height_;
    int width_;
};


#endif









