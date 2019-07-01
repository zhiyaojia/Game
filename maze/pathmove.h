#ifndef _PATHMOVE_H
#define _PATHMOVE_H
#include "object.h"

struct Pathmove
{
	Loc curr;
	int g;
	int h;
	Pathmove* prev;
  Pathmove(Loc temploc)
  {
    curr=temploc;
    prev=NULL;
    g=0;
  }
	Pathmove(Loc temploc, Pathmove *parent)
	{
		curr=temploc;
		prev=parent;
		g=parent->g+1;
	}
};


struct GreaterScore
{
    bool operator()(const Pathmove* m1, const Pathmove* m2) const
    {
        if((m1->g+m1->h)>(m2->g+m2->h))
      	  return true;
    	  else if((m1->g+m1->h)<(m2->g+m2->h))
          return false;
    	  else
   	 	  {
      		  if(m1->h>m2->h)
        		  return true;
      		  else if(m1->h<m2->h)
        		  return false;
      		  else
      		  {
        		  if(m1->g>=m2->g)
          	 		  return true;
        		  else
          			 return false;
      		  }
    	  }
    }
};


struct Different
{
  bool operator()(const Pathmove* m1, const Pathmove* m2) const
  {
    return (m1->curr<m2->curr);
  }
};

#endif







