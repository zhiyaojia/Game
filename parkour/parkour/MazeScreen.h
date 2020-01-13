#pragma once
#include "UIScreen.h"
#include "SDL/SDL.h"

class MazeScreen: public UIScreen
{
public:
	MazeScreen(class Game* game);

	void HandleKeyPress(int key);
};
