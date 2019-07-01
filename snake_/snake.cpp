#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <stdio.h>
#include <string.h>
#include <iomanip>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

using namespace std;

int pau;
char ch;
bool stop=false;
bool gameover=false;
int height=20, width=40;
int y=height/2, x=width/2;
vector<int> xdir, ydir;
int fruitx, fruity;
enum eDirection{LEFT, RIGHT, UP, DOWN};
eDirection dir;


int kbhit(void)
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

void Pause()
{
    pau++;
    if((pau%2)==0)
        stop=false;
    else
        stop=true;
}

void newfruit()
{
    fruitx=(rand()%(width-2))+1;
    fruity=(rand()%(height-2))+1;
}

void newgame()
{
    pau=0;
    for(int i=0; i<xdir.size(); i++)
    {
        xdir.pop_back();
        ydir.pop_back();
        i--;
    }
    y=height/2;
    x=width/2;
    gameover=false;
    xdir.push_back(x);
    ydir.push_back(y);
    newfruit();
}

void printboard()
{
    for(int i=0; i<=width; i++)
        cout<<"#";
    cout<<endl;
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            bool have=false;
            if(j==width-1)
                cout<<"#";
            else if(j==0)
                cout<<"#";
            for(int k=0; k<xdir.size(); k++)
            {
                if(i==ydir[k]&&j==xdir[k])
                {
                    cout<<"O";
                    have=true;
                    break;
                }
            }
            if(have)
                continue;
            else if(i==fruity&&j==fruitx)
                cout<<"F";
            else
                cout<<" ";
        }
        cout<<endl;
    }
    
    for(int i=0; i<=width; i++)
        cout<<"#";
    cout<<endl;
}

void move()
{
    if(!stop)
    {
        int i=xdir.size()-1;
        while(i>0)
        {
            xdir[i]=xdir[i-1];
            ydir[i]=ydir[i-1];
            i--;
        }
        switch(dir)
        {
            case LEFT:
                xdir[0]--;
                break;
            case RIGHT:
                xdir[0]++;
                break;
            case UP:
                ydir[0]--;
                break;
            case DOWN:
                ydir[0]++;
                break;
        }
    }
}

void lose()
{
    if(xdir[0]==0||ydir[0]==0||xdir[0]==width-1||ydir[0]==height-1)
        gameover=true;
    for(int i=1; i<xdir.size(); i++)
    {
        if(xdir[0]==xdir[i]&&ydir[0]==ydir[i])
            gameover=true;
    }
}

void logic()
{
    if(xdir[0]==fruitx&&ydir[0]==fruity)
    {
        newfruit();
        if(dir==LEFT)
        {
            int tempx=xdir.back();
            xdir.push_back(tempx+1);
            int tempy=ydir.back();
            ydir.push_back(tempy);
        }
        if(dir==RIGHT)
        {
            int tempx=xdir.back();
            xdir.push_back(tempx-1);
            int tempy=ydir.back();
            ydir.push_back(tempy);
        }
        if(dir==UP)
        {
            int tempx=xdir.back();
            xdir.push_back(tempx);
            int tempy=ydir.back();
            ydir.push_back(tempy+1);
        }
        if(dir==DOWN)
        {
            int tempx=xdir.back();
            xdir.push_back(tempx);
            int tempy=ydir.back();
            ydir.push_back(tempy-1);
        }
    }
}

void input()
{
    if(kbhit())
    {
        system("stty raw");
        ch = getchar();
        system("stty cooked");
        if(xdir.size()==1)
        {
            switch(ch)
            {
                case 'a':
                    dir=LEFT;
                    break;
                case 'd':
                    dir=RIGHT;
                    break;
                case 'w':
                    dir=UP;
                    break;
                case 's':
                    dir=DOWN;
                    break;
                case 'n':
                    gameover=true;
                    break;
                case 'p':
                    Pause();
                    break;
                default : // 可选的
                    break;
            }
        }
        else
        {
            switch(ch)
            {
                case 'a':
                    if(dir!=RIGHT)
                        dir=LEFT;
                    break;
                case 'd':
                    if(dir!=LEFT)
                        dir=RIGHT;
                    break;
                case 'w':
                    if(dir!=DOWN)
                        dir=UP;
                    break;
                case 's':
                    if(dir!=UP)
                        dir=DOWN;
                    break;
                case 'p':
                    Pause();
                    break;
                default : // 可选的
                    break;
            }
        }
    }
}

void over()
{
    system("clear");
    cout<<endl;
    cout<<endl;
    cout<<endl;
    cout
    <<"#########       #        #       #    #########       #########    #         #   #########   #########";
    cout<<endl;
    usleep(200000);
    cout
    <<"#              # #       ##     ##    #               #       #    #         #   #           #       #";
    cout<<endl;
    usleep(200000);
    cout
    <<"#             #   #      # #   # #    #               #       #     #       #    #           #########";
    cout<<endl;
    usleep(200000);
    cout
    <<"#    ####    #######     #  # #  #    #########       #       #      #     #     #########   # #      ";
    cout<<endl;
    usleep(200000);
    cout
    <<"#       #   #       #    #   #   #    #               #       #       #   #      #           #   #    ";
    cout<<endl;
    usleep(200000);
    cout
    <<"#       #  #         #   #       #    #               #       #        # #       #           #     #  ";
    cout<<endl;
    usleep(200000);
    cout
    <<"#########  #         #   #       #    #########       #########         #        #########   #       #";
    cout<<endl;
    usleep(200000);
    cout<<endl;
    cout<<endl;
    cout<<endl;
}


int main()
{
    system("clear");
    newgame();
    printboard();
    while(!gameover)
    {
        input();
        if(ch=='q')
            return 1;
        move();
        logic();
        lose();
        if(dir==LEFT||dir==RIGHT)
            usleep(100000);
        else if(dir==UP||dir==DOWN)
            usleep(200000);
        system("clear");
        printboard();
    }
    
    over();
    return 0;
}









