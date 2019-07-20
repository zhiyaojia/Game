#include"userInterface.h"
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<termios.h>
#include<unistd.h>
#define STDIN_FILENO 0

using namespace std;

Interface::Interface()
{
    pointer_={0,0,0,0,0};
    mode_={"static", "dynamic","random"};
    map_={"map_01", "map_02", "map_03", "map_04", "map_05", "random"};
    scene_={"garden", "forest", "city", "mountain", "random"};  
    //garden 🌺 forest 🌲 city 🏢 mountain 🏔️
    scene_demo={"🌺", "🌵", "🏢", "🏔️", "random"};
    role_={"😁 (dynamic)", "🤡", "🦁", "🐲", "🉑", "random"};
    guide_={"😇", "🈯", "🌀", "💩", "🧠", "random"};

    row=0;
    col=0;
}

void Interface::print_space(int width)
{
    for(int i=0; i<width; i++)
        cout<<" ";
    return;
}

void Interface::print_line(int height)
{
    for(int i=0; i<height; i++)
        cout<<endl;
    return;
}

void Interface::print_headline()
{
    print_line(4);
    print_space(32);
    cout<<"*           *          *       ************       **********"<<endl;
    print_space(32);
    cout<<"**         **         * *                *        *         "<<endl;
    print_space(32);
    cout<<"* *       * *        *   *              *         *         "<<endl;
    print_space(32);
    cout<<"*  *     *  *       *******            *          ********  "<<endl;
    print_space(32);
    cout<<"*   *   *   *      *       *          *           *         "<<endl;
    print_space(32);
    cout<<"*    * *    *     *         *       *             *         "<<endl;
    print_space(32);
    cout<<"*     *     *    *           *     ***********    **********"<<endl;
    print_line(3);
}

void Interface::display_menu(int row)
{
    print_line(1);
    print_space(32);
    cout<<"mode";
    if(row==0)
    {
        print_space(7);
        cout<<"◀  "<<menu[0]<<"  ▶"<<endl;
    }
    else
    {
        print_space(11);
        cout<<menu[0]<<endl;
    }

    print_space(32);
    cout<<"map";
    if(row==1)
    {
        print_space(14);
        cout<<"◀  "<<menu[1]<<"  ▶"<<endl; 
    }
    else
    {
        print_space(18);
        cout<<menu[1]<<endl;
    }

    print_space(32);
    cout<<"scene";
    if(row==2)
    {
        print_space(18);
        cout<<"◀  "<<menu[2]<<"  ▶"<<endl;
    }
    else
    {
        print_space(22);
        cout<<menu[2]<<endl;
    }

    print_space(32);
    cout<<"player";
    if(row==3)
    {
        print_space(24);
        cout<<"◀  "<<menu[3]<<"   ▶"<<endl;
    }
    else
    {
        print_space(28);
        cout<<menu[3]<<endl;
    } 

    print_space(32);
    cout<<"guide";
    if(row==4)
    {
        print_space(28);
        cout<<"◀  "<<menu[4]<<"   ▶"<<endl;
    }
    else
    {
        print_space(32);
        cout<<menu[4]<<endl;
    } 

    print_line(1);

    print_space(32);
    cout<<"press  J  start"<<endl;

    print_space(32);
    cout<<"       K  rule (better to know before start)"<<endl;

    print_space(32);  
    cout<<"       L  setting"<<endl; 
}


void Interface::control(int& row, int& col, vector<vector<string>> options, 
             vector<int> &pointer, Command dir)
{
    switch(dir)
    {
        case LEFT:
        {
            if(col==0)
                col=options[row].size()-1;
            else
                col--;
            pointer[row]=col;
            break;
        }
        case RIGHT:
        {
            if(col==options[row].size()-1)
                col=0;
            else
                col++;
            pointer[row]=col;
            break;
        }
        case UP:
        {
            if(row==0)
                row=options.size()-1;
            else
                row--;
            col=pointer[row];
            break;
        }
        case DOWN:
        {
            if(row==options.size()-1)
                row=0;
            else
                row++;
            col=pointer[row];
            break;
        }
        default:
            break;
    }
}

void Interface::print_rule()
{
    system("clear");
    print_headline();
    print_space(32);
    cout<<"规则：";
    print_line(2);
    print_space(32);
    cout<<"使用 w a s d或者方向键来移动人物走出迷宫，规定时间内未完成即失败"<<endl<<endl;
    print_space(32);
    cout<<"dynamic模式里迷宫会随着时间移动变化，static模式里迷宫一直保持不变"<<endl<<endl;
    print_space(32);
    cout<<"guide可以帮助玩家走出迷宫，玩家可在setting中调节次数"<<endl<<endl;
    print_space(32);
    cout<<"调节setting里的shadow可以控制可视范围"<<endl<<endl;
    print_space(32);
    cout<<"玩家可在setting中改变移动间隔来控制迷宫移动的速度"<<endl<<endl;
    print_line(2);
    print_space(80);
    cout<<"press Q to back"<<endl<<endl<<endl;

    char c;
    while(cin>>c)
    {
        if(c=='q')
            return;
    }
}

void Interface::display_smenu(int row, vector<string> s_menu)
{
    print_line(2);
    print_space(32);
    cout<<"shadow";
    if(row==0)
    {
        print_space(9);
        cout<<"◀  "<<s_menu[0]<<"  ▶"<<endl;
    }
    else
    {
        print_space(13);
        cout<<s_menu[0]<<endl;
    }

    print_space(32);
    cout<<"guide";
    if(row==1)
    {
        print_space(14);
        cout<<"◀  "<<s_menu[1]<<"  ▶"<<endl; 
    }
    else
    {
        print_space(18);
        cout<<s_menu[1]<<endl;
    }

    print_space(32);
    cout<<"总计时";
    if(row==2)
    {
        print_space(18);
        cout<<"◀  "<<s_menu[2]<<"  ▶"<<endl;
    }
    else
    {
        print_space(22);
        cout<<s_menu[2]<<endl;
    }

    print_space(32);
    cout<<"移动间隔";
    if(row==3)
    {
        print_space(21);
        cout<<"◀  "<<s_menu[3]<<"   ▶"<<endl;
    }
    else
    {
        print_space(25);
        cout<<s_menu[3]<<endl;
    } 

    print_line(2);
    print_space(80);
    cout<<"press Q to back"<<endl<<endl<<endl;
}

vector<string> Interface::more_setting()
{
    vector<vector<string>> setting=
    {
        {"ON", "OFF"},
        {"NO", "1", "2", "3", "4", "5", "∞"},
        {"10", "20", "30", "40", "50", "60", "∞"},
        {"5", "4", "3", "2", "1"}
    };
    vector<string> s_menu={"ON", "NO", "10", "5"};
    vector<int> s_pointer={0,0,0,0};

    int row_=0;
    int col_=0;
    Command s_dir;

    while(true)
    {
        system("clear");
        print_headline();
        display_smenu(row_, s_menu);

        s_dir=input();
        if(s_dir==OVER)
            break;
        control(row_, col_, setting, s_pointer, s_dir);
        if(s_menu[0]!="ON")
            whether_shadow=true;
        if(s_menu[2]!="10")
            whether_time=true;
    
        s_menu[row_]=setting[row_][col_];

    }    
    return s_menu;
}

Interface::Command Interface::input()
{
    char ch, d, e;
    cin>>ch;
    if(ch=='a')
        return LEFT;
    else if(ch=='d')
        return RIGHT;
    else if(ch=='w')
        return UP;
    else if(ch=='s')
        return DOWN;
    else if(ch=='j'||ch=='q')
        return OVER;
    else if(ch=='k')
    {
        print_rule();
        return BLANK;
    }
    else if(ch=='l')
        return SETTING;
    else if(ch==27) 
    {
        cin>>d>>e;
        if(d==91)
        {
            if(e==65)//w 
                return UP;
            else if(e==66)//s
                return DOWN;
            else if(e==67)//d
                return RIGHT;
            else if(e==68)//a
                return LEFT;
        }
    }
    return BLANK;
}

vector<string> Interface::run()
{
    srand( (unsigned)time( NULL ) );
    options_.push_back(mode_); options_.push_back(map_); options_.push_back(scene_);
    options_.push_back(role_); options_.push_back(guide_);

    for(int i=0; i<options_.size(); i++)
        menu.push_back(options_[i][pointer_[i]]);

    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);

    vector<string> temp_menu={"OFF", "3", "20", "2"};;

    while(true)
    {
        system("clear");
        print_headline();
        display_menu(row);

        dir_=input();
        if(dir_==OVER)
            break;
        else if(dir_==SETTING)
        {
            temp_menu=more_setting();
            whether_set=true;
        }
        control(row, col, options_, pointer_, dir_);
        
        menu[row]=options_[row][col];
    }
    for(int i=0; i<options_.size(); i++)
    {
        if(menu[i]=="random")
        {
            pointer_[i]=rand() % (options_[i].size()-1) + 0;
            menu[i]=options_[i][pointer_[i]];
        }
    }   
    menu[2]=scene_demo[pointer_[2]];

    menu.insert(menu.end(), temp_menu.begin(), temp_menu.end());

    if(!whether_set&&menu[0]=="dynamic")
    {
        menu[5]="ON";
        menu[6]="3";
    }

    if(!whether_time)
    {
        if(menu[0]=="dynamic")
            menu[7]="30";
        else
            menu[7]="60";
    }

    if(!whether_shadow)
    {
        if(menu[0]=="dynamic")
            menu[5]="ON";
        else
            menu[5]="OFF";  
    }
    
    for(int i=6; i<8; i++)
    {
        if(menu[i]=="∞")
            menu[i]="100000000";
    }

    if(menu[3]=="😁 (dynamic)")
        menu[3]="😁";

    return menu;
}
















