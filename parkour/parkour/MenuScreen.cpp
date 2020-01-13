#include "MenuScreen.h"
#include "Game.h"
#include <SDL/SDL.h>
#include "Renderer.h"
#include "Font.h"
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

	Texture* text1 = mFont->RenderText("Created by", Color::White, 20, true);
	mTextTextures.push_back(pair<Texture*, Vector2>(text1, Vector2(420.0f, -320.0f)));
    
    Texture* text2 = mFont->RenderText("Zhiyao Jia", Color::White, 20, true);
    mTextTextures.push_back(pair<Texture*, Vector2>(text2, Vector2(422.0f, -345.0f)));
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


















