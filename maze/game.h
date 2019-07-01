#ifndef _ELEMENT_H
#define _ELEMENT_H
#include <vector>
#include <string>
#include "object.h"
#include "board.h"
#include "pathsolver.h"

class Game
{
public:
    Game(std::vector<std::string> menu);
    ~Game();

    void input();
    void run();
    void output();

    bool logic();
    bool pause();
    bool keyhit();
    bool on();

private:
    void update_change(Loc loc, char ch);
    void timekeeping();

    int kbhit(void);
    bool righttime2(int time_lag_);
    bool righttime(int time_lag_, int &initial_time);

    bool classic_emoji;
    bool pauseindex;
    bool gameon;
    bool keyhit_;
    bool guide_on;

    char ch;

    std::vector<std::string> emoji={"🤢", "😱", "😨"};
    std::vector<std::string> life;

    int initial_time, temp_sec, temp_min, temp_time;
    int initial_time2, temp_sec2, temp_min2, temp_time2;
    int initial_time3;
    int time_amount;
    int time_lag1=1;    //timekeeping
    int time_lag2;    //maze change
    int time_lag3;   //change emoji
    int oldtime;    //time before pause
    int helpnum;

    Loc helper;
    Loc final_;

    Board board;
    Player player;
    Pathsolver solver;
};

#endif
