#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <string.h>
#include <iomanip>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

using namespace std;

//Gloabl Variables
char fruit='o';
char ch;
char board[100][100];

int xdir[4][5]=
{
    {-1,0,0}, {0,1,-1}, {1,0,0}, {0,-1,1}             //    4 shapes of Pacman
};                                                    //     1       2    1 2
int ydir[4][5]=                                       //    0    0    0    0
{                                                     //     2  2 1  1
    {0,-1,1}, {-1,0,0}, {0,1,-1}, {1,0,0}
};
int width=80, height=22;
int x, y;
int piece;
int fruitnum;
int counttime;
int difficulty=2;

bool whetherlose, whetherwin;
bool pauseindex;

enum direction{STOP, LEFT, RIGHT, UP, DOWN};
direction dir;

struct enemy
{
    int Sx;
    int Sy;
    bool touchfruit;
    bool alive;
    direction Sdir;
};
enemy A,B,C;


//Function prototypes
void setmap();
void printboard();
void printinformation(string s);
void printrules();
void newgame();

void input();
int kbhit(void);
void movecontrol(direction dir, int& x, int& y, bool ID);//true for pacman, false for enemy
bool canmove(int changex, int changey);
void correctposition(direction dir);

void logic();
void findpath(enemy& E, char aenemy1, char aenemy2);// another two enemies
pair<int, int> findMinNext(vector<pair<int, int> > compareLength);
void eatfruit();
void fruitcontrol();
void changebody(char c);
void gamecontrol();
bool kill(enemy E);
void cheat(string cheatcode);
bool collide(char target, int change_x, int change_y, pair<int, int> currentpoint);


//BASIC
void setmap()
{
    string a;
    a="################################################################################";
    for(int i=0; i<width; i++)
        board[0][i]=a[i];
    a="#                                                                           B  #";
    for(int i=0; i<width; i++)
        board[1][i]=a[i];
    a="#  S      oooooooooooooooooooooooooooooooooooooooooooooooooooooooooo           #";
    for(int i=0; i<width; i++)
        board[2][i]=a[i];
    a="#                                                                              #";
    for(int i=0; i<width; i++)
        board[3][i]=a[i];
    a="#                                                                  #           #";
    for(int i=0; i<width; i++)
        board[4][i]=a[i];
    a="#                                                                  #           #";
    for(int i=0; i<width; i++)
        board[5][i]=a[i];
    a="#                                                                  #           #";
    for(int i=0; i<width; i++)
        board[6][i]=a[i];
    a="######        ######               ######              ######      #           #";
    for(int i=0; i<width; i++)
        board[7][i]=a[i];
    a="     #        #****#               #****#              #****#      #           #";
    for(int i=0; i<width; i++)
        board[8][i]=a[i];
    a="     #        #****#               #****#              #****#      #           #";
    for(int i=0; i<width; i++)
        board[9][i]=a[i];
    a="######        #****#               #****#              #****#      #           #";
    for(int i=0; i<width; i++)
        board[10][i]=a[i];
    a="#             ######               ######              ######      #           #";
    for(int i=0; i<width; i++)
        board[11][i]=a[i];
    a="#                                                                  #           #";
    for(int i=0; i<width; i++)
        board[12][i]=a[i];
    a="#                                                                  #           #";
    for(int i=0; i<width; i++)
        board[13][i]=a[i];
    a="#                                                                              #";
    for(int i=0; i<width; i++)
        board[14][i]=a[i];
    a="#      oooooooooooooooooooooooo   #######   oooooooooooooooooooooooo           #";
    for(int i=0; i<width; i++)
        board[15][i]=a[i];
    a="#                                 #     #                                      #";
    for(int i=0; i<width; i++)
        board[16][i]=a[i];
    a="#        ##########################     ############################           #";
    for(int i=0; i<width; i++)
        board[17][i]=a[i];
    a="#                                                                              #";
    for(int i=0; i<width; i++)
        board[18][i]=a[i];
    a="#        ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo           #";
    for(int i=0; i<width; i++)
        board[19][i]=a[i];
    a="#  A                                                                        C  #";
    for(int i=0; i<width; i++)
        board[20][i]=a[i];
    a="################################################################################";
    for(int i=0; i<width; i++)
        board[21][i]=a[i];
}


void printboard()
{
    system("clear");
    if(board[B.Sy][B.Sx]==fruit)
        B.touchfruit=true;
    if(board[A.Sy][A.Sx]==fruit)
        A.touchfruit=true;
    if(board[C.Sy][C.Sx]==fruit)
        C.touchfruit=true;
    system("clear");
    cout<<"                                   Pacman"<<endl;
    board[B.Sy][B.Sx]='B';
    board[A.Sy][A.Sx]='A';
    board[C.Sy][C.Sx]='C';
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            /*if(board[i][j]==fruit)
             cout<<"💰";
             else if(board[i][j]=='A')
             cout<<"🐍";
             else if(board[i][j]=='B')
             cout<<"🐡";
             else if(board[i][j]=='C')
             cout<<"🐲";
             else if(board[i][j]=='X')
             cout<<"😂";
             else if(board[i][j]=='#')
             cout<<"🌵";
             else if(board[i][j]=='*')
             cout<<"☠";
             else*/
            cout<<board[i][j];
        }
        cout<<endl;
    }
    usleep(200000);
}

void printinformation(string s)
{
    system("clear");
    for(int i=0; i<11; i++)
        cout<<endl;
    cout<<"                                 "<<s<<endl;
    for(int i=0; i<11; i++)
        cout<<endl;
}

void printrules()
{
    system("clear");
    for(int i=0; i<6; i++)
        cout<<endl;
    cout<<"                Use W A S D to control the direction of pacman"<<endl<<endl;
    cout<<"                       Choose diffculty, please press:"<<endl<<endl;
    cout<<"                                  E: easy"<<endl<<endl;
    cout<<"                                  M: medium"<<endl<<endl;
    cout<<"                                  H: hard"<<endl<<endl;
    system("stty raw");
    ch = getchar();
    system("stty cooked");
    if(ch=='e')
        difficulty=3;
    if(ch=='m')
        difficulty=2;
    if(ch=='h')
        difficulty=1;
}


void newgame()
{
    setmap();
    
    piece=0;
    fruitnum=0;
    counttime=0;
    whetherwin=false;
    whetherlose=false;
    pauseindex=false;
    B.touchfruit=false;
    A.touchfruit=false;
    C.touchfruit=false;
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            
            if(board[i][j]=='B')
            {
                B.Sx=j;
                B.Sy=i;
            }
            if(board[i][j]=='A')
            {
                A.Sx=j;
                A.Sy=i;
            }
            if(board[i][j]=='C')
            {
                C.Sx=j;
                C.Sy=i;
            }
            if(board[i][j]=='S')
            {
                x=j;
                y=i;
                board[y][x]=' ';
            }
            if(board[i][j]==fruit)
                fruitnum++;
        }
    }
    changebody('X');
    printboard();
}

//INPUT
void input()
{
    changebody(' ');
    if(kbhit())
    {
        system("stty raw");
        ch = getchar();
        system("stty cooked");
        if(!pauseindex)
        {
            switch(ch)
            {
                case 'a':
                {
                    if(board[y][x]!='#')
                        correctposition(dir);
                    piece=2;
                    dir=LEFT;
                    movecontrol(dir, x, y, true);
                    break;
                }
                case 'd':
                {
                    if(board[y][x]!='#')
                        correctposition(dir);
                    piece=0;
                    dir=RIGHT;
                    movecontrol(dir, x, y, true);
                    break;
                }
                case 'w':
                {
                    if(board[y][x]!='#')
                        correctposition(dir);
                    piece=3;
                    dir=UP;
                    movecontrol(dir, x, y, true);
                    break;
                }
                case 's':
                {
                    if(board[y][x]!='#')
                        correctposition(dir);
                    piece=1;
                    dir=DOWN;
                    movecontrol(dir, x, y, true);
                    break;
                }
                case 'c':
                {
                    cout<<"please type in the cheating code: "<<endl;
                    string cheatcode;
                    cin>>cheatcode;
                    cheat(cheatcode);
                    break;
                }
                default:
                    break;
            }
            //usleep(60000);
        }
        if(ch=='p')
            pauseindex=pauseindex?false:true;
    }
    else
        movecontrol(dir, x, y, true);
}

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

void movecontrol(direction dir, int& x, int& y, bool ID)
{
    if(!pauseindex)
    {
        switch(dir)
        {
            case LEFT:
            {
                if((!ID&&board[y][x-1]!='#')||(ID&&canmove(-1,0)))
                    x--;
                break;
            }
            case RIGHT:
            {
                if((!ID&&board[y][x+1]!='#')||(ID&&canmove(1,0)))
                    x++;
                break;
            }
            case UP:
            {
                if((!ID&&board[y-1][x]!='#')||(ID&&canmove(0,-1)))
                    y--;
                break;
            }
            case DOWN:
            {
                if((!ID&&board[y+1][x]!='#')||(ID&&canmove(0,1)))
                    y++;
                break;
            }
            case STOP:
            {
                break;
            }
        }
    }
}

void correctposition(direction dir)
{
    switch(dir)
    {
        case LEFT:
        {
            if(board[y][x-1]=='#')
                x++;
            break;
        }
        case RIGHT:
        {
            if(board[y][x+1]=='#')
                x--;
            break;
        }
        case UP:
        {
            if(board[y-1][x]=='#')
                y++;
            break;
        }
        case DOWN:
        {
            if(board[y+1][x]=='#')
                y--;;
            break;
        }
        case STOP:
        {
            break;
        }
    }
}

void cheat(string cheatcode)
{
    if(cheatcode=="yuedu")
    {
        difficulty=100;
    }
    else
    {
        cout<<"invalid cheating code."<<endl;
    }
}

//LOGIC
void logic()
{
    if(!pauseindex)
    {
        findpath(B, 'A', 'C');
        if(counttime>30)
            findpath(A, 'B', 'C');
        if(counttime>50)
            findpath(C, 'A', 'B');
    }
    fruitcontrol();
    changebody('X');
    gamecontrol();
}

bool canmove(int changex, int changey)
{
    for(int i=0; i<3; i++)
    {
        if(board[y+ydir[piece][i]+changey][x+xdir[piece][i]+changex]=='#')
            return false;
    }
    return true;
}

bool canrotate(int temppiece)
{
    if(piece==0&&board[y][x+1]=='#'&&temppiece!=2)
        return false;
    if(piece==1&&board[y+1][x]=='#'&&temppiece!=3)
        return false;
    if(piece==2&&board[y][x-1]=='#'&&temppiece!=0)
        return false;
    if(piece==3&&board[y-1][x]=='#'&&temppiece!=1)
        return false;
    for(int i=0; i<3; i++)
    {
        if(board[y+ydir[temppiece][i]][x+xdir[temppiece][i]]=='#')
            return false;
    }
    return true;
}

bool collide(char target, int change_x, int change_y, pair<int, int> currentpoint)
{
    return board[currentpoint.second+change_y][currentpoint.first+change_x]==target;
}

void findpath(enemy& E, char aenemy1, char aenemy2)
{
    pair<int, int> currentpoint;
    currentpoint.first=E.Sx;
    currentpoint.second=E.Sy;
    pair<int ,int> nextpoint;
    vector<pair<int, int> > compareLength;
    if(!collide('#', 0, 1, currentpoint)&&!collide(aenemy1, 0, 1, currentpoint)&&!collide(aenemy2, 0, 1, currentpoint))
    {
        nextpoint.first=currentpoint.first;
        nextpoint.second=currentpoint.second+1;
        compareLength.push_back(nextpoint);
    }
    if(!collide('#', 0, -1, currentpoint)&&!collide(aenemy1, 0, -1, currentpoint)&&!collide(aenemy2, 0, -1, currentpoint))
    {
        nextpoint.first=currentpoint.first;
        nextpoint.second=currentpoint.second-1;
        compareLength.push_back(nextpoint);
    }
    if(!collide('#', 1, 0, currentpoint)&&!collide(aenemy1, 1, 0, currentpoint)&&!collide(aenemy2, 1, 0, currentpoint))
    {
        nextpoint.first=currentpoint.first+1;
        nextpoint.second=currentpoint.second;
        compareLength.push_back(nextpoint);
    }
    if(!collide('#', -1, 0, currentpoint)&&!collide(aenemy1, -1, 0, currentpoint)&&!collide(aenemy2, -1, 0, currentpoint))
    {
        nextpoint.first=currentpoint.first-1;
        nextpoint.second=currentpoint.second;
        compareLength.push_back(nextpoint);
    }
    pair<int, int> result=findMinNext(compareLength);
    if(E.touchfruit)
        board[E.Sy][E.Sx]=fruit;
    else
        board[E.Sy][E.Sx]=' ';
    
    if(counttime%difficulty==0)
    {
        E.Sx=result.first;
        E.Sy=result.second;
    }
    else
    {
        int r = rand() % (4 - 1 + 1) + 1;
        switch (r) {
            case 1:
                E.Sdir=UP;
                break;
            case 2:
                E.Sdir=DOWN;
                break;
            case 3:
                E.Sdir=RIGHT;
                break;
            case 4:
                E.Sdir=LEFT;
                break;
            default:
                break;
        }
        movecontrol(E.Sdir, E.Sx, E.Sy, false);
    }
}

pair<int, int> findMinNext(vector<pair<int, int> > compareLength)
{
    vector<pair<int, int> > identity; //first: pair index in compareLength, second: H value
    for(int i=0; i<compareLength.size(); i++)
    {
        pair<int, int> eachpair;
        eachpair.first=i;
        eachpair.second=abs(compareLength[i].first-x)+abs(compareLength[i].second-y);
        identity.push_back(eachpair);
    }
    
    int index=identity[0].first;
    int min=identity[0].second;
    for(int j=0; j<identity.size(); j++)
    {
        if(identity[j].second<min)
        {
            index=identity[j].first;
            min=identity[j].second;
        }
    }
    return compareLength[index];
}

void eatfruit()
{
    for(int i=0; i<3; i++)
    {
        if(board[y+ydir[piece][i]][x+xdir[piece][i]]==fruit)
        {
            fruitnum--;
            board[y+ydir[piece][i]][x+xdir[piece][i]]='X';
        }
    }
}

void fruitcontrol()
{
    eatfruit();
    if(B.touchfruit==true)
        B.touchfruit=false;
    if(A.touchfruit==true)
        A.touchfruit=false;
    if(C.touchfruit==true)
        C.touchfruit=false;
}

void changebody(char c)
{
    for(int i=0; i<3; i++)
        board[y+ydir[piece][i]][x+xdir[piece][i]]=c;
}

bool kill(enemy E)
{
    for(int i=0; i<3; i++)
    {
        if(E.Sy+1==y+ydir[piece][i]&&E.Sx==x+xdir[piece][i])
            return true;
        if(E.Sy-1==y+ydir[piece][i]&&E.Sx==x+xdir[piece][i])
            return true;
        if(E.Sy==y+ydir[piece][i]&&E.Sx+1==x+xdir[piece][i])
            return true;
        if(E.Sy==y+ydir[piece][i]&&E.Sx-1==x+xdir[piece][i])
            return true;
        if(E.Sy==y+ydir[piece][i]&&E.Sx==x+xdir[piece][i])
            return true;
    }
    return false;
}

void gamecontrol()
{
    counttime++;
    
    for(int i=0; i<3; i++)
    {
        if(B.Sy+1==y+ydir[piece][i]&&B.Sx==x+xdir[piece][i])
            whetherlose=true;
        if(B.Sy-1==y+ydir[piece][i]&&B.Sx==x+xdir[piece][i])
            whetherlose=true;
        if(B.Sy==y+ydir[piece][i]&&B.Sx+1==x+xdir[piece][i])
            whetherlose=true;
        if(B.Sy==y+ydir[piece][i]&&B.Sx-1==x+xdir[piece][i])
            whetherlose=true;
        if(B.Sy==y+ydir[piece][i]&&B.Sx==x+xdir[piece][i])
            whetherlose=true;
    }
    
    for(int i=0; i<3; i++)
    {
        if(A.Sy+1==y+ydir[piece][i]&&A.Sx==x+xdir[piece][i])
            whetherlose=true;
        if(A.Sy-1==y+ydir[piece][i]&&A.Sx==x+xdir[piece][i])
            whetherlose=true;
        if(A.Sy==y+ydir[piece][i]&&A.Sx+1==x+xdir[piece][i])
            whetherlose=true;
        if(A.Sy==y+ydir[piece][i]&&A.Sx-1==x+xdir[piece][i])
            whetherlose=true;
        if(A.Sy==y+ydir[piece][i]&&A.Sx==x+xdir[piece][i])
            whetherlose=true;
    }
    
    for(int i=0; i<3; i++)
    {
        if(C.Sy+1==y+ydir[piece][i]&&C.Sx==x+xdir[piece][i])
            whetherlose=true;
        if(C.Sy-1==y+ydir[piece][i]&&C.Sx==x+xdir[piece][i])
            whetherlose=true;
        if(C.Sy==y+ydir[piece][i]&&C.Sx+1==x+xdir[piece][i])
            whetherlose=true;
        if(C.Sy==y+ydir[piece][i]&&C.Sx-1==x+xdir[piece][i])
            whetherlose=true;
        if(C.Sy==y+ydir[piece][i]&&C.Sx==x+xdir[piece][i])
            whetherlose=true;
    }
    
    if(fruitnum==0)
        whetherwin=true;
}

int main()
{
    printrules();
    newgame();
    while(true)
    {
        while(!whetherwin&&!whetherlose)
        {
            input();
            if(ch=='q')
                return 0;
            logic();
            printboard();
        }
        if(whetherlose==true)
            printinformation("GG, bro!!!");
        if(whetherwin==true)
            printinformation("cool, man, you win!!!");
        cout<<"Press y to resume a new game       n to quit"<<endl;
        system("stty raw");
        char ch = getchar();
        system("stty cooked");
        cout<<endl;
        if(ch=='y')
            newgame();
        if(ch=='n')
            return 1;
    }
}





