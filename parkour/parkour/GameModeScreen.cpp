#include "GameModeScreen.h"
#include "Game.h"
#include "MenuScreen.h"
#include "Renderer.h"
#include "MazeScreen.h"
#include <iostream>
#include <SDL/SDL.h>

using namespace std;

GameModeScreen::GameModeScreen(Game* game):UIScreen(game)
{
	mNextButtonPos = Vector2(0, 140);
	mBackground = mGame->GetRenderer()->GetTexture("Assets/image/MenuBackground.png");
	AddClickButton("AdventureButton", "Assets/image/adventure.png", [this]()
	{
		mGame->ChangeState(Game::MenuToGame);
		mGame->SetGameMode(Game::Adventure);
		Close();
	});
	AddClickButton("MazeButton", "Assets/image/maze.png", [this]()
	{
		new MazeScreen(mGame);
		Close();
	});
	AddClickButton("PracticeButton", "Assets/image/gym.png", [this]() 
	{
   		mGame->ChangeState(Game::MenuToGame);
		mGame->SetGameMode(Game::Practice);
		Close();
	});
	AddClickButton("TutorialButton", "Assets/image/tutorial.png", [this]()
	{
		mGame->ChangeState(Game::MenuToGame);
		mGame->SetGameMode(Game::Tutorial);
		Close();
	});
	AddClickButton("BackButton", "Assets/image/MenuBackground.png", [this]()
	{
		new MenuScreen(mGame);
		Close();
	});
}


void GameModeScreen::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);
	if (key == SDLK_ESCAPE)
	{
		new MenuScreen(mGame);
		Close();
	}
}


















