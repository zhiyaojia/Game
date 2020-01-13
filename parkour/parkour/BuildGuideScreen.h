#pragma once
#include "UIScreen.h"
#include "SDL/SDL.h"
#include <string>

class BuildGuideScreen: public UIScreen
{
public:
	BuildGuideScreen(class Game* game);

	void HandleKeyPress(int key) override;
};
