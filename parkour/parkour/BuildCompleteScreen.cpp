#include "BuildCompleteScreen.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "PlayerMoveBuild.h"
#include "Player.h"
#include "Font.h"
#include <iostream>
#include <SDL/SDL.h>

using namespace std;

BuildCompleteScreen::BuildCompleteScreen(Game* game):UIScreen(game)
{	
	mNextButtonPos = Vector2(0.0f, -115);
	mBackground = mGame->GetRenderer()->GetTexture("Assets/image/SmallMenu.png");
	mBGPos = Vector2(0.0f, 0.0f);

	Texture* Text1 = mFont->RenderText("SuccessStore", Color::Green, 40);
	mTextTextures.push_back(pair<Texture*, Vector2>(Text1, Vector2(0.0f, 50.0f)));

	Texture* Text2 = mFont->RenderText(mGame->player->GetComponent<PlayerMoveBuild>()->GetFileName()
		, Color::Green, 40, true);
	mTextTextures.push_back(pair<Texture*, Vector2>(Text2, Vector2(0.0f, -15.0f)));

	timer = 0.0f;
	
	mButtonCover = mGame->GetRenderer()->GetTexture("Assets/image/ButtonCoverSetting.png");
	AddClickButton("BackButton", "EMPTY", [this](){ Close();});
}


void BuildCompleteScreen::Update(float deltaTime)
{
	timer += deltaTime;
	if(timer >= 2.5f)
	{
		Close();
	}
}















