#include "HUDAdventure.h"
#include "Texture.h"
#include <SDL/SDL_ttf.h>
#include "Font.h"
#include <string>
#include <iostream>
#include "Game.h"
#include "Actor.h"
#include "Renderer.h"
#include "Player.h"
#include "SecurityCamera.h"
#include "PlayerMoveAdventure.h"

using namespace std;

const Vector2 radarPos = Vector2(410.0f, -285.0f);
const float radarRange = 1500.0f;
const float radarRadius = 92.0f;

HUDAdventure::HUDAdventure(Actor* parent): UIComponent(parent)
{
	this->mFont = new Font(parent->GetGame(), false);
	this->mFont->Load("Assets/Font/HuaWenXiHei-1.ttf");

	this->mScorePos = Vector2(0.0f, 200.0f);

	string record = "";
	string score = "";
	string lose = "";
	if(mOwner->GetGame()->GetLanguage() == Game::English) 
	{
		record = "New Record";
		score = "Score: ";
		lose = "Lose";
	}
	else if(mOwner->GetGame()->GetLanguage() == Game::Chinese) 
	{
		record = "新纪录";
		score = "得分:  ";
		lose = "失败";
	}

	this->mScoreText = mFont->RenderText(score + "0", Color::Orange, 44);

	this->mRecordPos = Vector2(0.0f, 200.0f);
	this->mNewRecordText = mFont->RenderText(record, Color::DarkRed, 64);

	this->mTimerText = mFont->RenderText("3", Color::DarkRed, 64);
	this->mTimerPos = Vector2(0.0f, 50.0f);

	this->mLoseText = mFont->RenderText(lose, Color::DarkRed, 64);
	this->mLosePos = Vector2(0.0f, 50.0f);

	this->mHeartTextPos = Vector2(-400.0f, -250.0f);
	this->mHeartImagePos = Vector2(-450.0f, -250.0f);
	this->mHeartText = mFont->RenderText("x 0", Color::DarkRed, 32);
	this->mHeartImgae = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/image/heart.png");

	this->mBackgroundTexture = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/image/Radar.png");
	this->mArrowTexture = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/image/RadarArrow.png");
	this->mBlipTexture = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/image/Blip.png");
}


HUDAdventure::~HUDAdventure()
{
	this->mFont->Unload();
	delete mFont;
}


void HUDAdventure::Update(float deltaTime)
{
// Update Timer
	bool resume = mOwner->GetGame()->player->GetComponent<PlayerMoveAdventure>()->GetResume();
	if(!resume)
	{	
		showNewRecord = false;

		float mOldTimer = mTimer;
		mTimer -= deltaTime;
		if(mOldTimer >= 2.0f && mTimer <= 2.0f)
		{
			mTimerText->Unload();
			delete mTimerText;
			mTimerText = mFont->RenderText("2", Color::DarkRed, 64);
		}
		else if(mOldTimer >= 1.0f && mTimer <= 1.0f)
		{
			mTimerText->Unload();
			delete mTimerText;
			mTimerText = mFont->RenderText("1", Color::DarkRed, 64);
		}
		else if(mOldTimer >= 0.0f && mTimer <= 0.0f)
		{
			mTimerText->Unload();
			delete mTimerText;
			mTimerText = mFont->RenderText("Start", Color::DarkRed, 64);
		}
		else if(mTimer <= -0.5f)
		{
			mTimerText->Unload();
			delete mTimerText;
			mTimerText = mFont->RenderText("3", Color::DarkRed, 64);
			mOwner->GetGame()->player->GetComponent<PlayerMoveAdventure>()->SetResume(true);
			mTimer = 3.0f;
		}
	}
	else
	{
		int score = mOwner->GetGame()->player->GetComponent<PlayerMoveAdventure>()->GetScore();
	// Update Score
		if(!showNewRecord)
		{
			mScoreText->Unload();
			delete mScoreText;
			string scoretext = "";
			if(mOwner->GetGame()->GetLanguage() == Game::English) {scoretext = "Score: ";}
			else if(mOwner->GetGame()->GetLanguage() == Game::Chinese) {scoretext = "得分:  ";}
			if(!breakRecord)
			{
				this->mScoreText = mFont->RenderText(scoretext + to_string(score), Color::DarkRed, 44);
			}
			else
			{
				this->mScoreText = mFont->RenderText(scoretext + to_string(score), Color::Orange, 44);
			}
		}

	// Update Life
		mHeartText->Unload();
		delete mHeartText;
		mHeartText = mFont->RenderText("x " + to_string(
				mOwner->GetGame()->player->GetComponent<PlayerMoveAdventure>()->GetLife()), Color::DarkRed, 32);

	// Update Record
		if(!breakRecord)
		{
			int gamescore = mOwner->GetGame()->GetAdventureScore();
			if(score > gamescore)
			{
				breakRecord = true;
				showNewRecord = true;
			}
		}
		if(showNewRecord)
		{
			showRecordTimer -= deltaTime;
			if(showRecordTimer <= 0.0f)
			{
				showNewRecord = false;
			}
		}
	}
}


void HUDAdventure::Draw(class Shader* shader)
{
	bool resume = mOwner->GetGame()->player->GetComponent<PlayerMoveAdventure>()->GetResume();
	if(!resume)
	{
		DrawTexture(shader, mTimerText, mTimerPos);
	}
	else
	{
		if(showNewRecord)
		{
			DrawTexture(shader, mNewRecordText, mRecordPos);
		}
		else
		{
			if(mOwner->GetGame()->player->GetComponent<PlayerMoveAdventure>()->GetLose())
			{
				DrawTexture(shader, mLoseText, mLosePos);
			}
			else
			{
				DrawTexture(shader, mScoreText, mScorePos);
			}
		}

		DrawTexture(shader, mHeartText, mHeartTextPos);
		DrawTexture(shader, mHeartImgae, mHeartImagePos);

		DrawTexture(shader, mBackgroundTexture, radarPos);
		DrawTexture(shader, mArrowTexture, radarPos);

	// Security cameras
		Vector3 playerForward = mOwner->GetGame()->player->GetWorldForward();
		float angle = Math::Atan2(playerForward.y, playerForward.x);
		Matrix3 rotationMatrix = Matrix3::CreateRotation(angle);

		vector<SecurityCamera*> cameraArray = mOwner->GetGame()->cameraArray;
		Vector3 playerPos = mOwner->GetGame()->player->GetPosition();
		Vector2 player2D = Vector2(playerPos.y, playerPos.x);

	    for(auto camera: cameraArray)
		{
			Vector3 cameraPos = camera->GetPosition();
			Vector2 camera2D = Vector2(cameraPos.y, cameraPos.x);
			Vector2 dist = camera2D - player2D;

			Vector3 cameraForward = camera->GetWorldForward();
			float cameraAngle = Math::Atan2(cameraForward.x, cameraForward.y);
			
			if(dist.Length() > radarRange)
			{
				continue;
			}
			dist = Vector2(dist.x/radarRange, dist.y/radarRange);
			dist *= radarRadius;

			Vector2 cameraInRadar = Vector2::Transform(dist, rotationMatrix);
			DrawTexture(shader, mBlipTexture, cameraInRadar + radarPos, 1.0f, cameraAngle);
		}
	}
}




/*this->mCoinTextPos = Vector2(-400.0f, -300.0f);
	this->mCoinImagePos = Vector2(-450.0f, -300.0f);
    this->mCoinImage = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/image/Coin/coin1.png");
	this->mCoinText = mFont->RenderText("x " + to_string(mOwner->GetGame()->getCoin()));*/


// Update Coin Num
		/*int coinnum = mOwner->GetGame()->getCoin();
		mCoinImage->Unload();
		delete mCoinImage;
		string filename = "";
		if(coinnum >= 0 && coinnum <= 30) {filename = "Assets/image/Coin/coin1.png";}
		else if(coinnum >= 30 && coinnum <= 60) {filename = "Assets/image/Coin/coin2.png";}
		else if(coinnum >= 60 && coinnum <= 100) {filename = "Assets/image/Coin/coin3.png";}
		else if(coinnum >= 100 && coinnum <= 200) {filename = "Assets/image/Coin/coin4.png";}
		else {filename = "Assets/image/Coin/coin5.png";}
		this->mCoinImage = mOwner->GetGame()->GetRenderer()->GetTexture(filename);

		mCoinText->Unload();
		delete mCoinText;
		mCoinText = mFont->RenderText("x " + to_string(coinnum), Color::DarkRed, 32);*/

//DrawTexture(shader, mCoinText, mCoinTextPos);
		//DrawTexture(shader, mCoinImage, mCoinImagePos);



