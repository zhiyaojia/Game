#include "MenuScreen.h"
#include "Game.h"
#include <SDL/SDL.h>
#include "Renderer.h"
#include "SettingScreen.h"
#include "GameModeScreen.h"
#include <iostream>

using namespace std;

MenuScreen::MenuScreen(Game* game):UIScreen(game)
{
	mNextButtonPos = Vector2(0, 90);
	mBackground = mGame->GetRenderer()->GetTexture("Assets/image/MenuBackground.png");
	AddClickButton("StartButton", "EMPTY", [this]()
	{
		new GameModeScreen(mGame);
		Close();
	});
	AddClickButton("SettingButton", "EMPTY", [this]()
	{
		new SettingScreen(mGame, true);
	});
	AddClickButton("QuitButton", "EMPTY", [this]() {
   		mGame->ChangeState(Game::Quit);
   		Close();
	});
}


void MenuScreen::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);
	if (key == SDLK_ESCAPE)
	{
		mGame->ChangeState(Game::Quit);
		Close();
	}
}


















