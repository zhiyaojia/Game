#include "game.h"
#include "board.h"
#include "object.h"
#include "pathsolver.h"
#include "userInterface.h"
#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;

char music_name[]="00.mp3";

vector<string> lose=
{
" *************       *************         ************      ************     ***********",
"*                   *                      *          *      *          *     *         *",
"*                   *                      *          *      *          *     *         *",
"*                   *                      ************      *          *     *         *",
"*                   *                      *                 ************     *         *",
"*         ******    *         ******       *************     ***              *         *",
"*            *      *            *         *           *     *  ***           *         *",
"*            *      *            *     #   *           *     *     ***        *         *",
" *************       *************    ##   *************     *        ***     ***********"
};

vector<string> win=
{
"**************    *************     *************     **          ",
"**************    *************     *************     **          ",
"**                **         **     **         **     **          ",
"**                **         **     **         **     **          ",
"**                **         **     **         **     **          ",
"**                **         **     **         **     **          ",
"**                **         **     **         **     **          ",
"**************    *************     *************     ************",
"**************    *************     *************     ************",                      
"                                                                  ",
"          *          *                *            ***     **     ",
"         ***        ***              ***           ****    **     ",
"        ** **      ** **            ** **          ** **   **     ",
"       **   **    **   **          **   **         **  **  **     ",
"      **     **  **     **        *********        **   ** **     ",
"     **       ****       **      **       **       **    ****     ",
"    **         **         **    **         **      **     ***     "
};

void play_music() 
{
	char name[12]="play music/";
	strcat(name, music_name);
    system(name);
}

void printspace(int x)
{
	for(int i=0; i<x; i++)
		cout<<" ";
}

void printrow(int x)
{
	for(int i=0; i<x; i++)
		cout<<endl;
}

int main()
{
	Interface interface;
	vector<string> menu=interface.run();

	Game game(menu);

	if(menu[0]=="static")
	{
		game.firstoutput();
		char ch;
		while(true)
		{
			system("stty raw");
            ch = getchar();
            system("stty cooked");
			if(ch=='r')
				break;
		}
	}

	try
	{
		game.ini_time();
		while(game.logic())
	    {
	        game.input();
	        if(!game.pause())
	        	game.run();
	        if(game.keyhit())
	        	game.output();
	    }
	}
	catch(int a)
	{
		music_name[1]=menu[1][5];
		system("clear");
		printrow(5);
		thread t(play_music);
    	for(int i=0; i<win.size(); i++)
    	{
    		printspace(30);
    		cout<<win[i]<<endl;
    	}
    	t.join();
    	return 1;
	}
    catch(double b)
    {
    	system("clear");
		thread t(play_music);
		printrow(7);
    	for(int i=0; i<lose.size(); i++)
    	{
    		printspace(15);
    		cout<<lose[i]<<endl;
    	}
    	t.join();
    	return -1;
    }
    system("clear");
    cout<<"over"<<endl;
	return 0;
}










