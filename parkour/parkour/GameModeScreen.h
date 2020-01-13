#pragma once
#include "UIScreen.h"
#include "SDL/SDL.h"

class GameModeScreen: public UIScreen
{
public:
	GameModeScreen(class Game* game);

	void HandleKeyPress(int key) override;
};
