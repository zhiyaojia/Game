#include "QuitScreen.h"
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

QuitScreen::QuitScreen(Game* game):UIScreen(game)
{	
	mGame->ChangeState(Game::GamePaused);
	mGame->PauseGameSound();
	SetRelativeMouseMode(false);

	mNextButtonPos = Vector2(0.0f, 40.0f);
	mBackground = mGame->GetRenderer()->GetTexture("Assets/image/SmallMenu.png");
	mBGPos = Vector2(0.0f, 0.0f);

	mButtonCover = mGame->GetRenderer()->GetTexture("Assets/image/ButtonCoverSetting.png");

	AddClickButton("AgainButton", "EMPTY", [this](){ 
        mGame->ChangeState(Game::Gameplay);
        SetRelativeMouseMode(true);
		mGame->StartGameAgain();
   		Close();
	});

	AddClickButton("BackButton", "EMPTY", [this](){ 
		mGame->ChangeState(Game::GameToMenu);
   		Close();
	});
}


















