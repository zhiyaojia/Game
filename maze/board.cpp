#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <unistd.h>
#include <time.h>
#include <string>
#include "board.h"

using namespace std;

Board::Board(vector<string> menu)
{
    random_=menu[0]=="dynamic"?true:false;
    scene=menu[2];
    role_=menu[3];
    guide=menu[4];
    shadow_=menu[5]=="ON"?true:false;
    guide_=menu[6]=="ON"?true:false;

    char map_name[5]="map/";
    if(random_)
        strcat(map_name, "dynamic/");
    else
        strcat(map_name, "static/");

    char type[5]=".txt";
    const char *temp=menu[1].c_str();
    char name[7];
    strcpy(name, temp);
    strcat(map_name,name);
    strcat(map_name,type);
    ifstream file(map_name);
    if(file.fail())
    {
        cout<<"fail to read in the map."<<endl;
        return;
    }
    string storeint;
    string trash1, trash2;
    getline(file, storeint);
    stringstream getint;
    getint<<storeint;
    getint>>trash1>>width_>>trash2>>height_;
    if(getint.fail())
    {
        cout<<"fail to read in width and height."<<endl;
        return;
    }
   
    arr=new char*[height_];
    for(int i=0; i<height_; i++)
    {
        arr[i]=new char[width_];
    }
    random_arr=new bool*[height_];
    for(int i=0; i<height_; i++)
    {
        random_arr[i]=new bool[width_];
    }

    for(int i=0; i<height_; i++)
    {
        for(int j=0; j<width_; j++)
        {
            file>>arr[i][j];
            if(arr[i][j]=='.')
                arr[i][j]=' ';
        }
    }
    for(int i=0; i<height_; i++)
    {
        for(int j=0; j<width_; j++)
        {
            if(arr[i][j]=='S'||arr[i][j]=='F'||arr[i][j]=='*')
            {
                random_arr[i][j]=false;
                /*if(arr[i][j]=='*')
                    arr[i][j]=' ';*/
            }
            else
                random_arr[i][j]=true;
        }
    }
    if(random_)
        change();
    file.close();
}


Board::Board(const Board& b)
{
    height_=b.height();
    width_=b.width();
    arr=new char*[height_];
    for(int i=0; i<height_; i++)
        arr[i]=new char[width_];

    for(int i=0; i<height_; i++)
        for(int j=0; j<width_; j++)
            arr[i][j]=b.loc(i,j);
    random_arr=NULL;
}


Board::~Board()
{
    for(int i=0; i<height_; i++ )
        delete[] arr[i];
    delete[] arr;

    if(random_arr!=NULL)
    {
        for(int i=0; i<height_; i++ )
            delete[] random_arr[i];
        delete[] random_arr;
    }
    
}


void Board::print(Loc loc) 
{
    int marginx_=(128-(width_*2))/2;
    int marginy_=(35-(height_*2))/1.5;
    int edge=width_/20;
    if(edge<2)
        edge=2;
    system("clear");
    for(int k=0; k<marginy_-1; k++)
        cout<<endl;
    if(!shadow_)
    {
        for(unsigned int i=0; i<height_; i++)
        {
            for(int k=0; k<marginx_; k++)
                cout<<" ";
            for(unsigned int j=0; j<width_; j++)
            {
                if(arr[i][j]=='#')
                    cout<<scene;
                else if(arr[i][j]=='$')
                    cout<<guide;
                else if(arr[i][j]=='S')
                    cout<<role_;
                else if(arr[i][j]=='F')
                    cout<<"🎁";
                else if(arr[i][j]==' '||arr[i][j]=='*')
                    cout<<"  ";
            }
            cout<<endl;
        }
    }
    else
    {
        for(int i=0; i<height_; i++)
        {
            for(int k=0; k<marginx_; k++)
                cout<<" ";
            for(int j=0; j<width_; j++)
            {
                if(arr[i][j]=='$')
                    cout<<guide;
                else if(arr[i][j]=='F')
                    cout<<"🎁";
                else if((i>=(loc.row-edge)&&i<=(loc.row+edge))
                    &&(j>=(loc.col-edge)&&j<=(loc.col+edge)))
                {
                    if(arr[i][j]=='#')
                        cout<<scene;
                    else if(arr[i][j]=='S')
                        cout<<role_;
                    else if(arr[i][j]==' '||arr[i][j]=='*')
                        cout<<"  ";
                }
                else
                    cout<<"⬛";
            }
            cout<<endl;
        }
    }
    cout<<endl;
}


void Board::board_change(int row, int col, char ch) 
{
    arr[row][col]=ch;
}


void Board::change()
{
    if(!random_)
        return;
    srand((int)time(0));
    for(int i=1; i<height_-1; i++)
    {
        for(int j=1; j<width_-1; j++)
        {
            if(arr[i][j]=='S')
            {
                if(notedge(i+1, j))
                    arr[i+1][j]=' ';
                if(notedge(i-1, j))
                    arr[i-1][j]=' ';
                if(notedge(i, j+1))
                    arr[i][j+1]=' ';
                if(notedge(i, j-1))
                    arr[i][j-1]=' ';
                continue;
            }
            //if(arr[i][j]!='S'&&arr[i][j]!='F'&&arr[i][j]!='*'&&arr[i][j]!='$')
            if(random_arr[i][j]&&
                (arr[i][j]!='S'&&arr[i][j]!='F'&&arr[i][j]!='*'&&arr[i][j]!='$'))
            {
                arr[i][j]=' ';
                int temp=rand()%2; 
                if(temp==1)
                    arr[i][j]='#';
            }
        }
    }

}


void Board::change_player(string role)
{
    role_=role;
}


int Board::width() const
{
    return width_;
}


int Board::height() const
{
    return height_;
}


int Board::marginx() const
{
    return (128-(width_*2))/2;
}


char Board::loc(int row, int col) const
{
    return arr[row][col];
}


bool Board::random() const
{
    return random_;
}


bool Board::notedge(int row, int col)
{
    if(row==height_||row==0)
        return false;
    if(col==width_||col==0)
        return false;
    return true;
}




















