#include "HUD.h"
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

using namespace std;

const Vector2 radarPos = Vector2(410.0f, -285.0f);
const float radarRange = 1500.0f;
const float radarRadius = 92.0f;

HUD::HUD(Actor* parent): UIComponent(parent)
{
	this->mFont = new Font;
	this->mFont->Load("Assets/Inconsolata-Regular.ttf");

	this->mTimerText = mFont->RenderText("00:00.00");

	this->coinPos = Vector2(-450.0f, -300.0f);

	this->mCoinText = mFont->RenderText(to_string(mOwner->GetGame()->getCoin())+"/55");

	this->mCheckpointTexture = mFont->RenderText(mOwner->GetGame()->getCheckPointText());

	this->mBackgroundTexture = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/Radar.png");
	this->mArrowTexture = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/RadarArrow.png");
	this->mBlipTexture = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/Blip.png");
}


HUD::~HUD()
{
	this->mFont->Unload();
	delete mFont;
}


void HUD::Update(float deltaTime)
{
// game timer
	mOwner->GetGame()->updateTimer(deltaTime);
	float timer = mOwner->GetGame()->getTimer();

	mTimerText->Unload();
	delete mTimerText;

	int seconds = (int)timer;
	int ff = static_cast<int>((timer - seconds) * 100);
	int ss = seconds % 60;
	int mm = int(seconds / 60);


	string millsecond = "";
	if(ff < 10) 
	{
		millsecond += "0";
	}
	millsecond += to_string(ff);

	string second = "";
	if(ss < 10) 
	{
		second += "0";
	}
	second += to_string(ss);

	string minute = "";
	if(mm < 10) 
	{
		minute += "0";
	}
	minute += to_string(mm);

	string time = minute + ":" + second + ":" + millsecond;

	mTimerText = mFont->RenderText(time);


// checkpoint timer
	if(mOwner->GetGame()->getCheckPointTextTimer() <= 5.0f && mCheckpointTexture != NULL)
	{
		mOwner->GetGame()->updateCheckPointTextTimer(deltaTime);
	}
}


void HUD::Draw(class Shader* shader)
{
	DrawTexture(shader, mTimerText, Vector2(-420.0f, -325.0f));
	DrawTexture(shader, mCoinText, coinPos);
	if(mOwner->GetGame()->getCheckPointTextTimer() <= 5.0f && mCheckpointTexture != NULL)
	{
		DrawTexture(shader, mCheckpointTexture, Vector2(0, 0));
	}

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


void HUD::addCoin()
{
	mOwner->GetGame()->addCoin();
	int coinNum = mOwner->GetGame()->getCoin();
	if(coinNum > 9)
	{
		this->coinPos = Vector2(-443.0f, -300.0f);
	}

	mCoinText->Unload();
	delete mCoinText;

	mCoinText = mFont->RenderText(to_string(coinNum)+"/55");
}


void HUD::updateCheckpointText(string text)
{
	mOwner->GetGame()->resetCheckPointTextTimer();
	mCheckpointTexture = mFont->RenderText(text);
}






