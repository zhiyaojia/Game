#include "MazeScreen.h"
#include "Game.h"
#include "GameModeScreen.h"
#include "Renderer.h"
#include "OptionButton.h"
#include <iostream>
#include <vector>
#include <string>
#include <SDL/SDL.h>

using namespace std;

MazeScreen::MazeScreen(Game* game):UIScreen(game)
{
	mNextButtonPos = Vector2(-130, 120);
	mBackground = mGame->GetRenderer()->GetTexture("Assets/image/maze.png");

	vector<Option*> mMapOptions;
	vector<string> maps = mGame->mMazeMap;
	for(auto s: maps)
	{	
		string::size_type idx;
		idx = s.find(".json");
		if(idx == string::npos ) {continue;}

		string name = s;
		for(int i=0; i<name.length(); i++)
		{
			if(name[i] == '.')
			{
				name.erase(i, name.length()-i);
			}
		}

		mMapOptions.push_back(new Option(name, mFont, [this, name](){ mGame->SetMazeMap(name);}, true));
	}
	AddOptionButton("MazeMapButton", "EMPTY", mMapOptions);

	mNextButtonPos = Vector2(0, 50);
	AddClickButton("StartButton", "EMPTY", [this]()
	{
		mGame->ChangeState(Game::MenuToGame);
		mGame->SetGameMode(Game::Maze);
		Close();
	});
	AddClickButton("DIYButton", "EMPTY", [this]()
	{
		mGame->ChangeState(Game::MenuToGame);
		mGame->SetGameMode(Game::Build);
		Close();
	});
	AddClickButton("BackButton", "EMPTY", [this]()
	{
		new GameModeScreen(mGame);
		Close();
	});
}


void MazeScreen::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);
	if (key == SDLK_ESCAPE)
	{
		new GameModeScreen(mGame);
		Close();
	}
}



















