#pragma once
#include "UIScreen.h"
#include "SDL/SDL.h"
#include <string>

class BuildCompleteScreen: public UIScreen
{
public:
	BuildCompleteScreen(class Game* game);

	void Update(float deltaTime) override;

private:
	float timer;
};

