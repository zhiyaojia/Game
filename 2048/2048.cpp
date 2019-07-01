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

int board[4][4];
int predecessor[4][4];
pair<int, int> p;
int score, increase;
bool move1;

pair<int, int> findmax()
{
    pair<int, int> p1;
    p1.first=0;
    p1.second=0;
    int max=board[p1.first][p1.second];
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
        {
            if(board[i][j]>max)
            {
                p1.first=i;
                p1.second=j;
                max=board[i][j];
            }
        }
    return p1;
}

void swap(int *a, int* b)
{
    int temp=*a;
    *a=*b;
    *b=temp;
    //cout<<"Replace "<<*a<<" with "<<*b<<endl;
    if(*a!=0||*b!=0)
        move1=true;
    //cout<<cc<<endl;
}

void copy(int pre[][4], int current[][4])
{
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            pre[i][j]=current[i][j];
}

pair<int, int> Random()
{
    pair<int, int> p1;
    p1.first=rand()%4;
    p1.second=rand()%4;
    return p1;
}

bool merge(int row, int col, int nextrow, int nextcol)
{
    if(board[row][col]==board[nextrow][nextcol]&&board[row][col]!=0)
    {
        board[nextrow][nextcol]*=2;
        increase+=board[nextrow][nextcol];
        board[row][col]=0;
        move1=true;
        return true;
    }
    else
        return false;
}

void addpiece()
{
    p=Random();
    while(board[p.first][p.second]!=0)
        p=Random();
    int possi=rand()%10;
    if(possi==9)
        board[p.first][p.second]=4;
    else
        board[p.first][p.second]=2;
}

void print(int a)
{
    if(a==0)
        cout<<"        ";
    if(a==2)
        cout<<"     2  ";
    if(a==4)
        cout<<"     4  ";
    if(a==8)
        cout<<"     8  ";
    if(a==16)
        cout<<"    16  ";
    if(a==32)
        cout<<"    32  ";
    if(a==64)
        cout<<"    64  ";
    if(a==128)
        cout<<"   128  ";
    if(a==256)
        cout<<"   256  ";
    if(a==512)
        cout<<"   512  ";
    if(a==1024)
        cout<<"  1024  ";
    if(a==2048)
        cout<<"  2048  ";
    if(a==4096)
        cout<<"  4096  ";
    if(a==8192)
        cout<<"  8192  ";
}

bool check()
{
    int arr[6][6];
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
        {
            if(board[i][j]==0)
                return true;
            arr[i+1][j+1]=board[i][j];
        }
    for(int i=1; i<5; i++)
        for(int j=1; j<5; j++)
        {
            if(arr[i][j]==arr[i-1][j])
                return true;
            if(arr[i][j]==arr[i+1][j])
                return true;
            if(arr[i][j]==arr[i][j-1])
                return true;
            if(arr[i][j]==arr[i][j+1])
                return true;
        }
    return false;
}

void newgame()
{
    score=0;
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            board[i][j]=0;
    int count=0;
    while(true)
    {
        p=Random();
        if(board[p.first][p.second]==0)
        {
            board[p.first][p.second]=2;
            count++;
        }
        if(count==2)
            break;
    }
}



void printboard()
{
    for(int i=0; i<34;i++)
        cout<<"#";
    cout<<endl;
    for(int i=0; i<34; i++)
    {
        if(i==0||i==33)
            cout<<"#";
        else
            cout<<" ";
    }
    cout<<endl;
    for(int i=0; i<4; i++)
    {   cout<<"#";
        for(int j=0; j<4; j++)
            print(board[i][j]);
        cout<<"#"<<endl;
        for(int m=0; m<34; m++)
        {
            if(m==0||m==33)
                cout<<"#";
            else
                cout<<" ";
        }
        cout<<endl;
        if(i==3)
            break;
        for(int m=0; m<34; m++)
        {
            if(m==0||m==33)
                cout<<"#";
            else
                cout<<" ";
        }
        cout<<endl;
    }
    for(int i=0; i<34;i++)
        cout<<"#";
    cout<<endl<<endl;
    score+=increase;
    cout<<"Total Score is: "<<score;
    if(increase==0)
        cout<<endl;
    else
        cout<<"      +"<<increase<<endl;
}

void up()
{
    for(int j=0; j<4; j++)
    {
        for(int i=0; i<3; i++)
        {
            if(board[i][j]==0)
            {
                int raw=i, col=j;
                while(board[raw][col]==0)
                {
                    raw++;
                    if(raw==3)
                        break;
                }
                if(raw<=3)
                {
                    swap(&board[raw][col],&board[i][col]);
                    //cout<<"change ("<<raw<<","<<col<<") with ("<<i<<","<<col<<")"<<endl;
                }
                if(board[raw][col]==0&&board[i][col]==0)
                    break;
            }
        }
    }
    
    for(int j=0; j<4; j++)
    {
        for(int i=1; i<4; i++)
        {
            bool a=merge(i, j, i-1, j);
            if(a)
            {
                int raw=i, col=j;
                while(board[raw+1][col]!=0&&raw+1<=3)
                {
                    swap(&board[++raw][col],&board[raw][col]);
                    //cout<<"Change ("<<raw<<","<<col<<") with ("<<raw-1<<","<<col<<")"<<endl;
                }
                break;
            }
        }
    }
}

void down()
{
    for(int j=0; j<4; j++)
    {
        for(int i=3; i>=0; i--)
        {
            if(board[i][j]==0)
            {
                int raw=i, col=j;
                while(board[raw][col]==0)
                {
                    raw--;
                    if(raw==0)
                        break;
                }
                if(raw>=0)
                {
                    swap(&board[raw][col],&board[i][col]);
                    //cout<<"change ("<<raw<<","<<col<<") with ("<<i<<","<<col<<")"<<endl;
                }
                if(board[raw][col]==0&&board[i][col]==0)
                    break;
            }
        }
    }
    
    for(int j=0; j<4; j++)
    {
        for(int i=2; i>=0; i--)
        {
            bool a=merge(i, j, i+1, j);
            if(a)
            {
                int raw=i, col=j;
                while(board[raw-1][col]!=0&&raw-1>=0)
                {
                    swap(&board[--raw][col],&board[raw][col]);
                    //cout<<"Change ("<<raw<<","<<col<<") with ("<<raw+1<<","<<col<<")"<<endl;
                }
                break;
            }
        }
    }
}

void right()
{
    for(int i=0; i<4; i++)
    {
        for(int j=3; j>=0; j--)
        {
            if(board[i][j]==0)
            {
                int raw=i, col=j;
                while(board[raw][col]==0)
                {
                    col--;
                    if(col==0)
                        break;
                }
                if(col>=0)
                {
                    swap(&board[raw][col],&board[raw][j]);
                    //cout<<"change ("<<raw<<","<<col<<") with ("<<raw<<","<<j<<")"<<endl;
                }
                if(board[raw][col]==0&&&board[raw][j]==0)
                    break;
            }
        }
    }
    
    for(int i=0; i<4; i++)
    {
        for(int j=2; j>=0; j--)
        {
            bool a=merge(i, j, i, j+1);
            if(a)
            {
                int raw=i, col=j;
                while(board[raw][col-1]!=0&&col-1>=0)
                {
                    swap(&board[raw][--col],&board[raw][col]);
                    //cout<<"Change ("<<raw<<","<<col<<") with ("<<raw<<","<<col+1<<")"<<endl;
                }
                break;
            }
        }
    }
}

void left()
{
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            if(board[i][j]==0)
            {
                int raw=i, col=j;
                while(board[raw][col]==0)
                {
                    col++;
                    if(col==3)
                        break;
                }
                if(col<=3)
                {
                    swap(&board[raw][col],&board[raw][j]);
                    //cout<<"change ("<<raw<<","<<col<<") with ("<<raw<<","<<j<<")"<<endl;
                }
                if(board[raw][col]==0&&board[raw][j]==0)
                    break;
            }
        }
    }
    
    for(int i=0; i<4; i++)
    {
        for(int j=1; j<4; j++)
        {
            bool a=merge(i, j, i, j-1);
            if(a)
            {
                int raw=i, col=j;
                while(board[raw][col+1]!=0&&col+1<=3)
                {
                    swap(&board[raw][++col],&board[raw][col]);
                    //cout<<"Change ("<<raw<<","<<col<<") with ("<<raw<<","<<col-1<<")"<<endl;
                }
                break;
            }
        }
    }
}

void cheat(string a)
{
    if(a=="painkiller")
    {
        pair<int, int>p1=findmax();
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
            {
                if(i==p1.first&&j==p1.second)
                    continue;
                else
                    board[i][j]=0;
            }
        move1=true;
    }
    else if(a=="doubleme")
    {
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                board[i][j]*=2;
    }
    else if(a=="chopme")
    {
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
            {
                if(board[i][j]!=2)
                    board[i][j]/=2;
            }
    }
    else if(a=="bigbang")
    {
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                board[i][j]*=8;
    }
    else if(a=="predecessor")
    {
        copy(board,predecessor);
    }
    else
    {
        cout<<"invalid cheating code."<<endl;
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
    srand(time(0));
    
    int count=0;
    while(true)
    {
        p=Random();
        if(board[p.first][p.second]==0)
        {
            board[p.first][p.second]=2;
            count++;
        }
        if(count==2)
            break;
    }
    
    system("clear");
    printboard();
    while(true)
    {
        
        while(1)
        {
            increase=0;
            move1=false;
            system("stty raw");
            char ch = getchar();
            system("stty cooked");
            cout<<endl;
            if(!check())
            {
                break;
            }
            if(ch=='c')
            {
                cout<<"please type in the cheating code: "<<endl;
                string a;
                cin>>a;
                cheat(a);
            }
            else
            {
                copy(predecessor, board);
                if(ch=='n')
                    newgame();
                if(ch=='w')
                    up();
                if(ch=='s')
                    down();
                if(ch=='a')
                    left();
                if(ch=='d')
                    right();
                if(ch=='q')
                    return 2;
            }
            if(move1)
                addpiece();
            system("clear");
            printboard();
        }
        over();
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
    return 0;
    
}






