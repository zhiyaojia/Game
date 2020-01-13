#pragma once
#include "UIScreen.h"

class AdventurePauseScreen: public UIScreen
{
public:
	AdventurePauseScreen(class Game* game);
	~AdventurePauseScreen();

	void HandleKeyPress(int key) override;
};
