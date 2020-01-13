#include "AdventureRespawnScreen.h"
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

AdventureRespawnScreen::AdventureRespawnScreen(Game* game):UIScreen(game)
{	
	mGame->ChangeState(Game::GamePaused);
	mGame->PauseGameSound();
	SetRelativeMouseMode(false);

	mNextButtonPos = Vector2(0.0f, -115);
	mBackground = mGame->GetRenderer()->GetTexture("Assets/image/SmallMenu.png");
	mBGPos = Vector2(0.0f, 0.0f);

	mButtonCover = mGame->GetRenderer()->GetTexture("Assets/image/ButtonCoverSetting.png");

    int newScore = mGame->player-> GetComponent<PlayerMoveAdventure>()->GetScore();
	int oldScore = mGame->GetAdventureScore();

	if(game->player->GetComponent<PlayerMoveAdventure>()->BreakRecord())
	{
		Texture* newRecordText = mFont->RenderText("NewRecord", Color::Orange, 64);
		Texture* scoreText = mFont->RenderText(to_string(newScore), Color::Orange, 64, true);
		mTextTextures.push_back(pair<Texture*, Vector2>(newRecordText, Vector2(0.0f, 85.0f)));
		mTextTextures.push_back(pair<Texture*, Vector2>(scoreText, Vector2(0.0f, 0.0f)));
	}
	else
	{
		Texture* newScoreText = mFont->RenderText("ScoreText", Color::DarkRed, 56);
		Texture* newScoreNum = mFont->RenderText(to_string(newScore), Color::DarkRed, 56, true);
		Texture* oldScoreText = mFont->RenderText("RecordText", Color::Orange, 24);
		Texture* oldScoreNum = mFont->RenderText(to_string(oldScore), Color::Orange, 24, true);
		mTextTextures.push_back(pair<Texture*, Vector2>(newScoreText, Vector2(-70.0f, 80.0f)));
		mTextTextures.push_back(pair<Texture*, Vector2>(newScoreNum, Vector2(70.0f, 80.0f)));
		mTextTextures.push_back(pair<Texture*, Vector2>(oldScoreText, Vector2(-30.0f, 0.0f)));
		mTextTextures.push_back(pair<Texture*, Vector2>(oldScoreNum, Vector2(80.0f, 0.0f)));
	}

	AddClickButton("BackButton", "EMPTY", [this](){ 
		mGame->ChangeState(Game::GameToMenu);
   		Close();
	});
}


















