#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <stdio.h>
#include <iomanip>
#include <vector>
#include <deque>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include "game.h"

using namespace std;

Game::Game(vector<string> menu):
board(menu), player(0,0),solver(board)
{
    emoji.push_back("🤑");
    emoji.push_back("😱");
    emoji.push_back("😨");

    pauseindex=false;
    gameon=true;
    keyhit_=false;
    guide_on=false;
    classic_emoji=false;

    helpnum=atoi(menu[6].c_str()); 
    if(helpnum>5)
        life.push_back("∞");
    else 
    {
        for(int i=0; i<helpnum; i++)
            life.push_back("❤️");
    }

    time_lag2=atoi(menu[8].c_str());
    time_amount=atoi(menu[7].c_str());

    int Srow, Scol, Frow, Fcol;
    for(int i=0; i<board.height(); i++)
        for(int j=0; j<board.width(); j++)
        {
            if(board.loc(i,j)=='S')
            {
                Srow=i;
                Scol=j;
            }
            if(board.loc(i,j)=='F')
            {
                Frow=i;
                Fcol=j;
            }
        }

    if(menu[3]=="😁")
    {
        classic_emoji=true;
        time_lag3=time_amount/4;
    }

    player.loc.row=Srow;
    player.loc.col=Scol;
    final_.row=Frow;
    final_.col=Fcol;
    helper.row=-1;
    helper.col=-1;
    output();
}


Game::~Game()
{

}


int Game::kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}


void Game::input()
{
    keyhit_=false;
    if(kbhit())
    {
        cin>>ch;
        if(ch=='q')
        {
            gameon=false;
            return;
        }
        if(!pauseindex)
        {
            update_change(player.loc, ' ');
            keyhit_=true;
            char d,e;
            if(ch=='a')
            {
                if(player.loc.col>0
                            &&board.loc(player.loc.row, player.loc.col-1)!='#')
                            player.loc.col--;
            }
            else if(ch=='d')
            {
                if(player.loc.col<board.width()-1
                            &&board.loc(player.loc.row, player.loc.col+1)!='#')
                            player.loc.col++;
            }
            else if(ch=='w')
            {
                if(player.loc.row>0
                            &&board.loc(player.loc.row-1, player.loc.col)!='#')
                            player.loc.row--;
            }
            else if(ch=='s')
            {
                if(player.loc.row<board.height()-1
                            &&board.loc(player.loc.row+1, player.loc.col)!='#')
                            player.loc.row++;
            }
            else if(ch=='j'&&helpnum>0&&!guide_on)
            {
                board.guide_=true;
                helpnum--;
                life.pop_back();
                guide_on=true;

                solver.run(player.loc, final_);
                deque<Loc> path=solver.getSolution();
                helper=path[path.size()/2];
                /*if(path.size()>=15)
                    helper=path[9];
                else 
                    helper=path.back();*/
                update_change(helper, '$');
            }
            else if(ch==27) 
            {
                cin>>d>>e;
                if(d==91)
                {
                    if(e==65)//w 
                    { 
                        if(player.loc.row>0
                            &&board.loc(player.loc.row-1, player.loc.col)!='#')
                            player.loc.row--;
                    }
                    else if(e==66)//s
                    {
                        if(player.loc.row<board.height()-1
                            &&board.loc(player.loc.row+1, player.loc.col)!='#')
                            player.loc.row++;
                    }
                    else if(e==67)//d
                    {
                        if(player.loc.col<board.width()-1
                            &&board.loc(player.loc.row, player.loc.col+1)!='#')
                            player.loc.col++;
                    }
                    else if(e==68)//a
                    {
                        if(player.loc.col>0
                            &&board.loc(player.loc.row, player.loc.col-1)!='#')
                            player.loc.col--;
                    }
                }
            }
            else if(ch=='p')
            {
                pauseindex=true;
                oldtime=time_amount;
            }
        
            update_change(player.loc, 'S');
        }
        else if(ch=='p')
        {
            pauseindex=pauseindex?false:true;
            time_amount=oldtime;
        }
    }
}


bool Game::logic()
{
    if(!gameon)
        return false;
    if(player.loc==final_)
    {
        board.change_player("😎");
        output();
        usleep(700000);
        throw 1;
        return false;
    }
    if(time_amount<=0)
    {
        if(classic_emoji)
            board.change_player("😵");
        output();
        usleep(700000);
        throw 0.5;
        return false;
    }
    return true;
}


void Game::run()
{
    if(!logic())
        return;
    if(righttime(time_lag2, initial_time2)&&board.random())
    {
        board.change();
        output();
    }
    else if(righttime(time_lag1, initial_time))
    {
        time_amount-=time_lag1;
        output();
    }

    if(righttime(time_lag3, initial_time3)&&classic_emoji)
    {
        if(emoji.size()>=1)
        {
            board.change_player(emoji[emoji.size()-1]);
            emoji.pop_back();
        }
        output();
    }

    if(player.loc==helper)
    {
        //update_change(helper, ' ');
        guide_on=false;
        helper.row=-1;
        helper.col=-1;
    }
}


void Game::output()
{
        board.print(player.loc);
        int marginx=board.marginx();
        int space=47-marginx;
        for(int k=0; k<marginx; k++)
            cout<<" ";
        cout<<"life: ";
        if(helpnum>5)
        {
            cout<<"∞";
            space-=1;
        }
        else
        {
            for(int i=0; i<life.size(); i++)
                cout<<life[i]<<" ";
            space-=(3*life.size());
        }
        for(int j=0; j<space; j++)
            cout<<" ";
        cout<<"Time Left: ";
        if(time_amount<=100)
            cout<<time_amount<<"s"<<endl;
        else
            cout<<"∞"<<endl;
        for(int k=0; k<marginx; k++)
            cout<<" ";
        cout<<"press j for guide"<<endl;
}


void Game::firstoutput()
{
    board.print(player.loc);
    for(int i=0; i<50; i++)
        cout<<" ";
    cout<<"press r when ready"<<endl;
}



void Game::update_change(Loc loc, char ch)
{
    board.board_change(loc.row, loc.col, ch);
}


bool Game::pause()
{
    return pauseindex;
}


bool Game::keyhit()
{
    return keyhit_;
}


bool Game::righttime(int time_lag_, int &initial_time)
{
    time_t tc = time(NULL);
    tm* temp= localtime(&tc);
    temp_sec=temp->tm_sec;
    temp_min=temp->tm_min;
    temp_time=temp_min*60+temp_sec;
    if(temp_time-initial_time==time_lag_)
    {
        initial_time+=time_lag_;
        return true;
    }
    return false;
}

bool Game::righttime2(int time_lag_)
{
    time_t tc2 = time(NULL);
    tm* temp2= localtime(&tc2);
    temp_sec2=temp2->tm_sec;
    temp_min2=temp2->tm_min;
    temp_time2=temp_min*60+temp_sec2;
    if(temp_time2-initial_time2==time_lag_)
    {
        initial_time2+=time_lag_;
        return true;
    }
    return false;
}


void Game::ini_time()
{
    time_t tt = time(NULL);
    tm* t= localtime(&tt);
    int initial_sec=t->tm_sec;
    int initial_min=t->tm_min;
    initial_time=initial_min*60+initial_sec;
    initial_time2=initial_time;
    initial_time3=initial_time;
    oldtime=initial_time;
}







