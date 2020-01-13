#pragma once
#include "UIScreen.h"
#include "SDL/SDL.h"
#include <string>

class BuildAlertScreen: public UIScreen
{
public:
	BuildAlertScreen(class Game* game, std::string alert);
};
