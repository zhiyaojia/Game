#include "board.h"

#ifndef _OBJECT_H
#define _OBJECT_H

class Loc
{
public:
    int row;
    int col;
    Loc()
    {
        row=0;
        col=0;
    }
    Loc(int row_, int col_)
    {
        row=row_;
        col=col_;
    }
    bool operator==(const Loc& loc2) const
    {
    	return (this->row==loc2.row&&this->col==loc2.col);
    }
    bool operator<(const Loc& loc2) const
    {
    	if(this->row<loc2.row)
    		return true;
    	if(this->col<loc2.col)
    		return true;
    	return false;
    }
};


class Player
{
public:
    Loc loc;
    Player(int x_, int y_)
    {
        loc.row = x_;
        loc.col = y_;
    }
    ~Player()
    {
        
    }
};

#endif









