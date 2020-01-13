#pragma once
#include "UIScreen.h"
#include "SDL/SDL.h"

class MenuScreen: public UIScreen
{
public:
	MenuScreen(class Game* game);

	void HandleKeyPress(int key) override;
};
