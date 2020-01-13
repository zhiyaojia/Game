#include "BuildPauseScreen.h"
#include "Game.h"
#include "Player.h"
#include "PlayerMoveBuild.h"
#include "Renderer.h"
#include "SettingScreen.h"
#include "BuildAlertScreen.h"
#include "BuildCompleteScreen.h"
#include "BuildGuideScreen.h"
#include <SDL/SDL.h>
#include <iostream>

using namespace std;

BuildPauseScreen::BuildPauseScreen(Game* game):UIScreen(game)
{
	mNextButtonPos = Vector2(0, 130);
	mBackground = mGame->GetRenderer()->GetTexture("Assets/image/PauseBG.png");
	mGame->ChangeState(Game::GamePaused);
	mGame->PauseGameSound();
	SetRelativeMouseMode(false);
	AddClickButton("ResumeButton", "EMPTY", [this]() 
	{
		SetRelativeMouseMode(true);
		mGame->ChangeState(Game::Gameplay);
		Close();
	});
	AddClickButton("GuideButton", "EMPTY", [this]() 
	{
		new BuildGuideScreen(mGame);
	});
	AddClickButton("FinishButton", "EMPTY", [this]()
	{	
		if(mGame->player->GetComponent<PlayerMoveBuild>()->mStartPoint == nullptr)
		{
			new BuildAlertScreen(mGame, "StartPointAlert");
		}
		else if(mGame->player->GetComponent<PlayerMoveBuild>()->mFinalPoint == nullptr)
		{
			new BuildAlertScreen(mGame, "FinalPointAlert");
		}
		else
		{
			mGame->player->GetComponent<PlayerMoveBuild>()->WriteBlockToFile();
			new BuildCompleteScreen(mGame);
		}
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

BuildPauseScreen::~BuildPauseScreen()
{
	mGame->ResumeGameSound();
}

void BuildPauseScreen::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);
	if (key == SDLK_ESCAPE)
	{
		SetRelativeMouseMode(true);
		mGame->ChangeState(Game::Gameplay);
		Close();
	}
}





















