#pragma once
#include "UIScreen.h"

class BuildPauseScreen: public UIScreen
{
public:
	BuildPauseScreen(class Game* game);
	~BuildPauseScreen();

	void HandleKeyPress(int key) override;
};
