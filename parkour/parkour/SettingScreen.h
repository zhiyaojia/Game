#pragma once
#include "UIScreen.h"
#include "SDL/SDL.h"

class SettingScreen: public UIScreen
{
public:
	SettingScreen(class Game* game, bool language);

	void HandleKeyPress(int key) override;

	void AddVolumeButton(const std::string& name, const std::string& background, std::vector<class Option*> option);
};
