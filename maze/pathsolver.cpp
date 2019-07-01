#include "pathsolver.h"
#include "pathmove.h"
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;


Pathsolver::Pathsolver(const Board& board):b(board)
{
	
}


Pathsolver::~Pathsolver()
{

}


void Pathsolver::run(Loc start_, Loc final_)
{
	solution_.clear();

	vector<Pathmove*> allmove;
	GreaterScore comp;
	vector<Pathmove*> openlist;
	make_heap(openlist.begin(), openlist.end(), comp);
	set<Loc> closedlist;

	Pathmove* start=new Pathmove(start_);
	allmove.push_back(start);
	openlist.push_back(start);
	push_heap(openlist.begin(), openlist.end(), comp);
	closedlist.insert(start_);
	Pathmove* final=NULL;

	while(!openlist.empty())
	{

		Pathmove* temp=openlist.front();
		if(temp->curr==final_)
		{
			final=temp->prev;
			break;
		}

		pop_heap(openlist.begin(), openlist.end(), comp);
		openlist.pop_back();

		set<Loc> successor=potentialMoves(temp->curr);
		
		vector<Pathmove*> nextmove;
		set<Loc>::iterator it;
		for(it=successor.begin(); it!=successor.end(); it++)
		{
			set<Loc>::iterator last=closedlist.find(*it);
			if(last!=closedlist.end())
				continue;
			Pathmove* next=new Pathmove(*it,temp);
			next->h=getHeuristic(*it, final_);
			nextmove.push_back(next);
			allmove.push_back(next);
		}
		
		
		for(unsigned int i=0; i<nextmove.size(); i++)
		{
			bool inopenlist=false;
			for(unsigned int j=0; j<openlist.size(); j++)
			{
				if(openlist[j]->curr==nextmove[i]->curr)
				{
					if(comp(openlist[j],nextmove[i]))
					{
						openlist[j]->prev=nextmove[i]->prev;
						inopenlist=true;
						break;
					}
				}
			}
			if(!inopenlist)
			{
				openlist.push_back(nextmove[i]);
				push_heap(openlist.begin(), openlist.end(), comp);
			}
		}
		closedlist.insert(temp->curr);
	}

	while(final!=start)
	{
		solution_.push_front(final->curr);
		final=final->prev;
	}
	
	for(unsigned int i=0; i<allmove.size(); i++)
		delete allmove[i];

}


set<Loc> Pathsolver::potentialMoves(Loc curr)
{
	set<Loc> nextMove;
	Loc next;
	if(b.loc(curr.row-1, curr.col)!='#')
	{
		next.row=curr.row-1;
		next.col=curr.col;
		nextMove.insert(next);
	}
	if(b.loc(curr.row+1, curr.col)!='#')
	{
		next.row=curr.row+1;
		next.col=curr.col;
		nextMove.insert(next);
	}
	if(b.loc(curr.row, curr.col-1)!='#')
	{
		next.row=curr.row;
		next.col=curr.col-1;
		nextMove.insert(next);
	}
	if(b.loc(curr.row, curr.col+1)!='#')
	{
		next.row=curr.row;
		next.col=curr.col+1;
		nextMove.insert(next);
	}
	return nextMove;
}


int Pathsolver::getHeuristic(Loc curr, Loc final_)
{
	int horizontal_dist=abs(curr.row-final_.row);
	int vertical_dist=abs(curr.col-final_.col);
	return (horizontal_dist+vertical_dist);
}


deque<Loc> Pathsolver::getSolution()
{
	return solution_;
}


void Pathsolver::printPath()
{
	for(unsigned int i=0; i<solution_.size(); i++)
		b.board_change(solution_[i].row, solution_[i].col, '*');
}












