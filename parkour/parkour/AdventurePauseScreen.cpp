#include "AdventurePauseScreen.h"
#include "PlayerMoveAdventure.h"
#include "Game.h"
#include "Player.h"
#include "Renderer.h"
#include "SettingScreen.h"
#include <SDL/SDL.h>
#include <iostream>

using namespace std;

AdventurePauseScreen::AdventurePauseScreen(Game* game):UIScreen(game)
{
	mNextButtonPos = Vector2(0, 90);
	mBackground = mGame->GetRenderer()->GetTexture("Assets/image/PauseBG.png");
	mGame->ChangeState(Game::GamePaused);
	mGame->PauseGameSound();
	SetRelativeMouseMode(false);
	AddClickButton("ResumeButton", "EMPTY", [this]() 
	{
		SetRelativeMouseMode(true);
		mGame->ChangeState(Game::Gameplay);
		mGame->player->GetComponent<PlayerMoveAdventure>()->SetResume(false);
		Close();
	});
	AddClickButton("SettingButton", "EMPTY", [this]()
	{
		new SettingScreen(mGame, false);
	});
	AddClickButton("QuitButton", "EMPTY", [this]() 
	{
   		mGame->ChangeState(Game::GameToMenu);
   		Close();
	});
}

AdventurePauseScreen::~AdventurePauseScreen()
{
	mGame->ResumeGameSound();
}

void AdventurePauseScreen::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);
	if (key == SDLK_ESCAPE)
	{
		SetRelativeMouseMode(true);
		mGame->ChangeState(Game::Gameplay);
		Close();
	}
}





















