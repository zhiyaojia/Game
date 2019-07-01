#ifndef _PATHSOLVER_H
#define _PATHSOLVER_H
#include "board.h"
#include "object.h"
#include <deque>
#include <set>

class Pathsolver
{
public:	
	Pathsolver(const Board& board);
	~Pathsolver();
	std::set<Loc> potentialMoves(Loc curr);
	int getHeuristic(Loc curr, Loc final_);
	void run(Loc S, Loc F);
	std::deque<Loc> getSolution();
	void printPath();

private:
	Board b;
	std::deque<Loc> solution_;
};



#endif