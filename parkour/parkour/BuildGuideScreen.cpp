#include "BuildGuideScreen.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "Player.h"
#include "Font.h"
#include <iostream>
#include <SDL/SDL.h>

using namespace std;

BuildGuideScreen::BuildGuideScreen(Game* game):UIScreen(game)
{	
	mNextButtonPos = Vector2(0.0f, -190);
	mBackground = mGame->GetRenderer()->GetTexture("Assets/image/SmallMenu.png");
	mBGPos = Vector2(0.0f, 0.0f);

	mButtonCover = mGame->GetRenderer()->GetTexture("Assets/image/ButtonCoverSetting.png");

	Vector2 pos = Vector2(0.0f, 170.0f);

	for(int i=1; i<=11; i++)
	{
		Texture* operation = mFont->RenderText("Operation" + to_string(i), Color::White, 20);
		mTextTextures.push_back(pair<Texture*, Vector2>(operation, pos));
		pos.y -= 30.0f;
	}

	AddClickButton("BackButton", "EMPTY", [this](){ Close();});
}


void BuildGuideScreen::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);
	if (key == SDLK_ESCAPE)
	{
		Close();
	}
}















