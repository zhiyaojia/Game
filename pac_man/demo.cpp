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
char ch;
char board[100][100];

int width=80, height=22;
int x, y;
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
void movecontrol(direction dir, int& x, int& y);

void logic();
void findpath(enemy& E, char aenemy1, char aenemy2);// another two enemies
pair<int, int> findMinNext(vector<pair<int, int> > compareLength);
void fruitcontrol();
void gamecontrol();
void cheat(string cheatcode);
bool collide(char target, int change_x, int change_y, pair<int, int> currentpoint);
//BASIC
void setmap()
{
    string a="################################################################################";
    for(int i=0; i<width; i++)
        board[0][i]=a[i];
    a="# X    .............................................................        B  #";
    for(int i=0; i<width; i++)
        board[1][i]=a[i];
    a="#       .                #####################                                 #";
    for(int i=0; i<width; i++)
        board[2][i]=a[i];
    a="#         .              #                   #                                 #";
    for(int i=0; i<width; i++)
        board[3][i]=a[i];
    a="#           .            #####################                     #           #";
    for(int i=0; i<width; i++)
        board[4][i]=a[i];
    a="#             .............................................        #           #";
    for(int i=0; i<width; i++)
        board[5][i]=a[i];
    a="#                                                                  #           #";
    for(int i=0; i<width; i++)
        board[6][i]=a[i];
    a="######        ######              ######               ######      #           #";
    for(int i=0; i<width; i++)
        board[7][i]=a[i];
    a="     #        #    #              #    #               #    #      #           #";
    for(int i=0; i<width; i++)
        board[8][i]=a[i];
    a="     #        #    #              #    #               #    #      #           #";
    for(int i=0; i<width; i++)
        board[9][i]=a[i];
    a="######        #    #              #    #               #    #      #           #";
    for(int i=0; i<width; i++)
        board[10][i]=a[i];
    a="#             ######              ######               ######      #           #";
    for(int i=0; i<width; i++)
        board[11][i]=a[i];
    a="#                                                                  #           #";
    for(int i=0; i<width; i++)
        board[12][i]=a[i];
    a="#                ..................................                #           #";
    for(int i=0; i<width; i++)
        board[13][i]=a[i];
    a="#                        #####################                     #           #";
    for(int i=0; i<width; i++)
        board[14][i]=a[i];
    a="#                        #                   #                     #           #";
    for(int i=0; i<width; i++)
        board[15][i]=a[i];
    a="#                        #####################                     #           #";
    for(int i=0; i<width; i++)
        board[16][i]=a[i];
    a="#                                                                  #           #";
    for(int i=0; i<width; i++)
        board[17][i]=a[i];
    a="#        .............................................................         #";
    for(int i=0; i<width; i++)
        board[18][i]=a[i];
    a="#                                                                              #";
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
    board[y][x]='X';
    if(board[B.Sy][B.Sx]=='.')
        B.touchfruit=true;
    if(board[A.Sy][A.Sx]=='.')
        A.touchfruit=true;
    if(board[C.Sy][C.Sx]=='.')
        C.touchfruit=true;
    system("clear");
    board[B.Sy][B.Sx]='B';
    board[A.Sy][A.Sx]='A';
    board[C.Sy][C.Sx]='C';
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
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
            if(board[i][j]=='.')
                fruitnum++;
            if(board[i][j]=='X')
            {
                x=j;
                y=i;
            }
        }
    }
    printboard();
}

//INPUT
void input()
{
    board[y][x]=' ';
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
                    dir=LEFT;
                    break;
                }
                case 'd':
                {
                    dir=RIGHT;
                    break;
                }
                case 'w':
                {
                    dir=UP;
                    break;
                }
                case 's':
                {
                    dir=DOWN;
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
        }
        if(ch=='p')
            pauseindex=pauseindex?false:true;
    }
    movecontrol(dir, x, y);
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

void movecontrol(direction dir, int& x, int& y)
{
    if(!pauseindex)
    {
        switch(dir)
        {
            case LEFT:
            {
                if(board[y][x-1]!='#')
                    x--;
                break;
            }
            case RIGHT:
            {
                if(board[y][x+1]!='#')
                    x++;
                break;
            }
            case UP:
            {
                if(board[y-1][x]!='#')
                    y--;
                break;
            }
            case DOWN:
            {
                if(board[y+1][x]!='#')
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
    gamecontrol();
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
        board[E.Sy][E.Sx]='.';
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
        movecontrol(E.Sdir, E.Sx, E.Sy);
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

void fruitcontrol()
{
    if(board[y][x]=='.')
        fruitnum--;
    if(B.touchfruit==true)
        B.touchfruit=false;
    if(A.touchfruit==true)
        A.touchfruit=false;
    if(C.touchfruit==true)
        C.touchfruit=false;
}

void gamecontrol()
{
    counttime++;
    
    if(B.Sy+1==y&&B.Sx==x)
        whetherlose=true;
    if(B.Sy-1==y&&B.Sx==x)
        whetherlose=true;
    if(B.Sy==y&&B.Sx+1==x)
        whetherlose=true;
    if(B.Sy==y&&B.Sx-1==x)
        whetherlose=true;
    if(B.Sy==y&&B.Sx==x)
        whetherlose=true;
    
    if(A.Sy+1==y&&A.Sx==x)
        whetherlose=true;
    if(A.Sy-1==y&&A.Sx==x)
        whetherlose=true;
    if(A.Sy==y&&A.Sx+1==x)
        whetherlose=true;
    if(A.Sy==y&&A.Sx-1==x)
        whetherlose=true;
    if(A.Sy==y&&A.Sx==x)
        whetherlose=true;
    
    if(C.Sy+1==y&&C.Sx==x)
        whetherlose=true;
    if(C.Sy-1==y&&C.Sx==x)
        whetherlose=true;
    if(C.Sy==y&&C.Sx+1==x)
        whetherlose=true;
    if(C.Sy==y&&C.Sx-1==x)
        whetherlose=true;
    if(C.Sy==y&&C.Sx==x)
        whetherlose=true;
    
    if(fruitnum==0)
        whetherwin=true;
}

int main()
{
    printrules();
    newgame();
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
}





