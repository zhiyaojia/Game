#include "BuildAlertScreen.h"
#include "Game.h"
#include "Renderer.h"
#include "OptionButton.h"
#include "Texture.h"
#include "PlayerMoveAdventure.h"
#include "Player.h"
#include "Font.h"
#include <iostream>
#include <SDL/SDL.h>

using namespace std;

BuildAlertScreen::BuildAlertScreen(Game* game, std::string alert):UIScreen(game)
{	
	mNextButtonPos = Vector2(0.0f, -115);
	mBackground = mGame->GetRenderer()->GetTexture("Assets/image/SmallMenu.png");
	mBGPos = Vector2(0.0f, 0.0f);

	mButtonCover = mGame->GetRenderer()->GetTexture("Assets/image/ButtonCoverSetting.png");

	Texture* alertText = mFont->RenderText(alert, Color::DarkRed, 40);
	mTextTextures.push_back(pair<Texture*, Vector2>(alertText, Vector2(0.0f, 0.0f)));

	AddClickButton("BackButton", "EMPTY", [this](){ Close();});
}


















