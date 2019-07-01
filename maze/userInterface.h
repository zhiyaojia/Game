#ifndef _USERINTERFACE_H
#define _USERINTERFACE_H
#include<string>
#include<vector>

class Interface
{
public:
	std::vector<std::string> run();

	Interface();
	enum Command{LEFT, RIGHT, UP, DOWN, BLANK, OVER, SETTING};
	void print_space(int width);
	void print_line(int height);
	void print_headline();
	void print_rule();

	Command input();
	void control(int& row, int& col, std::vector<std::vector<std::string>> options, 
                 std::vector<int> &pointer, Command dir);
	
	void display_menu(int row);
	void display_smenu(int row, std::vector<std::string> s_menu);
	std::vector<std::string> more_setting();

private:
	std::vector<std::string> menu;
	std::vector<std::vector<std::string> > options_;
	std::vector<int> pointer_;
	std::vector<std::string> mode_;
    std::vector<std::string> map_;
    std::vector<std::string> scene_; 
    std::vector<std::string> scene_demo;  
    std::vector<std::string> role_;
    std::vector<std::string> guide_;

	Command dir_;

	int row, col;

	bool whether_set=false;
	bool whether_time=false;
	bool whether_shadow=false;
};

#endif

//	menu information
//		0 mode
//		1 map
//		2 scene
//		3 player
//		4 guide
//		5 shadow
//		6 guide(bool)
//		7 time_amount
//		8 time_lag




























