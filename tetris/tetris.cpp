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

//global variables

int xdir[28][4]=
{
    {0,-1,0,1},{0,0,1,0},{-1,0,1,0},{0,-1,0,0},//T
    {0,0,0,0},{-1,0,1,2},{0,0,0,0},{-1,0,1,2},//I
    {-1,-1,0,1},{0,1,0,0},{-1,0,1,1},{0,0,-1,0},//J
    {1,-1,0,1},{0,0,0,1},{-1,0,1,-1},{-1,0,0,0},//L
    {0,1,-1,0},{-1,-1,0,0},{0,1,-1,0},{0,0,1,1},//S
    {-1,0,0,1},{0,0,-1,-1},{-1,0,0,1},{1,0,1,0},//Z
    {0,1,0,1},{0,1,0,1},{0,1,0,1},{0,1,0,1}//O
};
int ydir[28][4]=
{
    {-1,0,0,0},{-1,0,0,1},{0,0,0,1},{-1,0,0,1},//T
    {-1,0,1,2},{0,0,0,0},{-1,0,1,2},{0,0,0,0},//I
    {-1,0,0,0},{-1,-1,0,1},{0,0,0,1},{-1,0,1,1},//J
    {-1,0,0,0},{-1,-0,1,1},{0,0,0,1},{-1,-1,0,1},//L
    {0,0,1,1},{-1,0,0,1},{-1,-1,0,0},{-1,0,0,1},//S
    {0,0,1,1},{-1,0,0,1},{-1,-1,0,0},{-1,0,0,1},//Z
    {0,0,1,1},{0,0,1,1},{0,0,1,1},{0,0,1,1}//O
};

int piece;
int nextpiece;
int x,y;
int skippiece;
int prex[4], prey[4];
int board[18][8];
int smallboard[5][5];
double movedown;
char ch;
bool pauseindex;
bool gameover;

//Function Prototype
void newpiece();//generate a new piece
void newgame();
void printrules();
void printsmallboard(int col);
void printboard();

void input();
bool fit(int pp, int state);
void correct(int& tempx);
void cheat(string cheatcode);

bool land(int tempy);
bool vanish();
bool gamecontrol();
void showpiece(int tempx, int tempy);
void logic();



//functions
//BASIC
void newpiece()
{
    for(int i=0; i<4; i++)
        smallboard[2+ydir[nextpiece][i]][2+xdir[nextpiece][i]]=0;
    y=1;
    movedown=y;
    x=(rand()%4)+2;
    piece=nextpiece;
    nextpiece=rand()%27;
    for(int i=0; i<4; i++)
        smallboard[2+ydir[nextpiece][i]][2+xdir[nextpiece][i]]=1;
}

void newgame()
{
    skippiece=3;
    gameover=false;
    pauseindex=false;
    for(int i=0; i<18; i++)
        for(int j=0; j<8; j++)
            board[i][j]=0;
    for(int i=0; i<5; i++)
        for(int j=0; j<5; j++)
            smallboard[i][j]=0;
    for(int i=0; i<4; i++)
    {
        prex[i]=0;
        prey[i]=0;
    }
    do
    {
        nextpiece=rand()%27;
    }
    while(nextpiece>15&&nextpiece<24);
    newpiece();
    for(int i=0; i<4; i++)
        board[y+ydir[piece][i]][x+xdir[piece][i]]=2;
    printboard();
}

void printrules()
{
    system("clear");
    cout<<endl;
    cout<<"         "
    <<"Welcome to play Tetris"<<endl;
    cout<<endl;
    cout<<"         "
    <<"Here are the game rules: "<<endl;
    cout<<"                   "
    <<"--Press W to rotate the piece"<<endl;
    cout<<"                   "
    <<"--Press A to move left"<<endl;
    cout<<"                   "
    <<"--Press D to move right"<<endl;
    cout<<"                   "
    <<"--Press S to move down"<<endl;
    cout<<"                   "
    <<"--Press J to drop the piece"<<endl;
    cout<<"                   "
    <<"--Press K to skip the current piece"<<endl;
    cout<<"                   "
    <<"--Press P to pause"<<endl;
    cout<<"                   "
    <<"--Press N to resume a new game"<<endl;
    cout<<"                   "
    <<"--Press Q to quit the game"<<endl;
    cout<<endl<<endl<<endl;
    cout<<"              "
    <<"Are U ready?       "<<"Press Y to START"<<endl;
    cout<<endl;
    cout<<"                                                    "
    <<"---created by Jasper Jia"<<endl;
    system("stty raw");
    ch = getchar();
    system("stty cooked");
    if(ch=='y')
    {
        system("clear");
        return;
    }
}

void printsmallboard(int col)
{
    if(col>6)
        return;
    else
    {
        if(col==0||col==5)
        {
            cout<<"        ";
            for(int i=0; i<7; i++)
                cout<<".";
            return;
        }
        else if(col==6)
        {
            cout<<"        ";
            cout<<skippiece<<" chances to skip this piece.";
        }
        else
        {
            cout<<"        "<<".";
            for(int i=0; i<5; i++)
            {
                if(smallboard[col][i]==1)
                    cout<<"O";
                else if(smallboard[col][i]==0)
                    cout<<" ";
            }
            cout<<".";
            return;
        }
    }
}

void printboard()
{
    cout<<endl;
    cout<<"        俄罗斯方块"<<endl;
    cout<<"        ";
    for(int i=0; i<10; i++)
        cout<<"#";
    cout<<"        "<<"NextPiece: ";
    cout<<endl;
    
    for(int i=0; i<18; i++)
    {
        cout<<"        "<<"#";
        for(int j=0; j<8; j++)
        {
            if(board[i][j]==1)
                cout<<"A";
            else if(board[i][j]==2)
                cout<<"O";
            else if(board[i][j]==3)
                cout<<"=";
            else if(board[i][j]==4)
            {
                cout<<"*";
                board[i][j]=0;
            }
            else
                cout<<" ";
        }
        cout<<"#";
        printsmallboard(i);
        cout<<endl;
    }
    cout<<"        ";
    for(int i=0; i<10; i++)
        cout<<"#";
    cout<<endl;
}



//INPUT
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

void correct(int& tempx)
{
    switch(piece)
    {
        case 1:
            if(x==1||board[y][x-1]==1)
                tempx++;
            break;
        case 3:
            if(x==7||board[y][x+1]==1)
                tempx--;
            break;
        case 4:
            if(x==6||board[y][x+2]==1)
                tempx--;
            if(x==7||board[y][x+1]==1)
                tempx-=2;
            if(x==0||board[y][x-1]==1)
                tempx++;
            break;
        case 6:
            if(x==6||board[y][x+2]==1)
                tempx--;
            if(x==7||board[y][x+1]==1)
                tempx-=2;
            if(x==0||board[y][x-1]==1)
                tempx++;
            break;
        case 9:
            if(x==0||board[y][x-1]==1)
                tempx++;
            break;
        case 11:
            if(x==7||board[y][x+1]==1)
                tempx--;
            break;
        case 13:
            if(x==0||board[y][x-1]==1)
                tempx++;
            break;
        case 15:
            if(x==7||board[y][x+1]==1)
                tempx--;
            break;
        case 17:
            if(x==7||board[y][x+1]==1)
                tempx--;
            break;
        case 19:
            if(x==0||board[y][x-1]==1)
                tempx++;
            break;
        case 21:
            if(x==7||board[y][x+1]==1)
                tempx--;
            break;
        case 23:
            if(x==0||board[y][x-1]==1)
                tempx++;
            break;
        default:
            break;
    }
    return;
}

bool fit(int pp, int state, int tempx, int tempy)
{
    for(int i=0; i<4; i++)
    {
        if(board[tempy+ydir[pp][i]][tempx+xdir[pp][i]]==state)
            return false;
    }
    return true;
}

void input()
{
    if(kbhit())
    {
        for(int i=0; i<4; i++)
            board[y+ydir[piece][i]][x+xdir[piece][i]]=0;
        system("stty raw");
        ch = getchar();
        system("stty cooked");
        if(!pauseindex)
        {
            if(ch=='w')
            {
                int tempx=x;
                int pp=piece+1;
                if(pp%4==0)
                    pp-=4;
                correct(tempx);
                if(fit(pp, 1, x, y))
                {
                    piece=pp;
                    x=tempx;
                }
            }
            else if(ch=='a')
            {
                for(int i=0; i<4; i++)
                {
                    if(x+xdir[piece][i]==0||board[y+ydir[piece][i]][x-1+xdir[piece][i]]==1)
                        return;
                }
                x--;
            }
            else if(ch=='d')
            {
                for(int i=0; i<4; i++)
                {
                    if(x+xdir[piece][i]==7||board[y+ydir[piece][i]][x+1+xdir[piece][i]]==1)
                        return;
                }
                x++;
            }
            else if(ch=='s')
            {
                if(land(y))
                {
                    y++;
                    movedown=y;
                }
            }
            else if(ch=='j')
            {
                while(land(y))
                {
                    y++;
                }
            }
            else if(ch=='k')
            {
                if(skippiece>0)
                {
                    newpiece();
                    skippiece--;
                }
                
            }
            else if(ch=='c')
            {
                cout<<"please type in the cheating code: "<<endl;
                string cheatcode;
                cin>>cheatcode;
                cheat(cheatcode);
            }
        }
        if(ch=='p')
            pauseindex=pauseindex?false:true;
    }
    if(!pauseindex&&!kbhit())
    {
        for(int i=0; i<4; i++)
            board[y+ydir[piece][i]][x+xdir[piece][i]]=false;
        if(land(y))
        {
            movedown+=0.25;
            y=movedown;
        }
    }
}

void cheat(string cheatcode)
{
    if(cheatcode=="painkiller")
    {
        skippiece=100;
    }
    else if(cheatcode=="vanish")
    {
        for(int i=17; i>=15; i--)
            for(int j=0; j<8; j++)
                board[i][j]=3;
        system("clear");
        printboard();
        usleep(200000);
        for(int k=17; k>=3; k--)
            for(int j=0; j<8; j++)
                board[k][j]=board[k-3][j];
    }
    else if(cheatcode=="predecessor")
    {
        for(int i=0; i<4; i++)
        {
            board[prey[i]][prex[i]]=0;
        }
    }
    else
    {
        cout<<"invalid cheating code."<<endl;
    }
}



//LOGIC
bool land(int tempy)
{
    for(int i=0; i<4; i++)
    {
        if(board[tempy+(ydir[piece][i])+1][x+xdir[piece][i]]==1||tempy+(ydir[piece][i])==17)
            return false;
    }
    return true;
}

bool vanish(int col)
{
    int num=0;
    for(int i=0; i<8; i++)
    {
        if(board[col][i]==1)
            num++;
        if(num==8)
        {
            return true;
        }
    }
    return false;
}

bool gamecontrol()
{
    for(int i=0; i<8; i++)
    {
        if(board[0][i]==1)
            return true;
    }
    return false;
}

void showpiece(int tempx, int tempy)
{
    while(land(tempy))
        tempy++;
    if(fit(piece, 2, tempx, tempy))
    {
        for(int i=0; i<4; i++)
            board[tempy+ydir[piece][i]][tempx+xdir[piece][i]]=4;
    }
}

void logic()
{
    showpiece(x, y);
    if(!land(y))
    {
        for(int i=0; i<4; i++)
            board[y+ydir[piece][i]][x+xdir[piece][i]]=1;
        for(int i=0; i<4; i++)
        {
            prex[i]=x+xdir[piece][i];
            prey[i]=y+ydir[piece][i];
        }
        newpiece();
    }
    else
    {
        for(int i=0; i<4; i++)
            board[y+ydir[piece][i]][x+xdir[piece][i]]=2;
    }
    for(int i=17; i>=0; i--)
    {
        if(vanish(i))
        {
            printboard();
            for(int j=0; j<8; j++)
                board[i][j]=3;
            system("clear");
            printboard();
            usleep(200000);
            for(int k=i; k>=1; k--)
            {
                for(int j=0; j<8; j++)
                {
                    board[k][j]=board[k-1][j];
                }
            }
        }
    }
    gameover=gamecontrol();
}

int main()
{
    srand(time(0));
    printrules();
    while(true)
    {
        newgame();
        while(!gameover)
        {
            input();
            logic();
            if(ch=='n')
                break;
            if(ch=='q')
                return 0;
            system("clear");
            printboard();
            usleep(200000);
        }
        cout<<"Game Over"<<endl;
        cout<<"Press y to resume a new game       n to quit"<<endl;
        cin>>ch;
        if(ch=='y')
            continue;
        if(ch=='n')
            return 1;
    }
}













