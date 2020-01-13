#include "HUDMaze.h"
#include "Texture.h"
#include <SDL/SDL_ttf.h>
#include "Font.h"
#include <string>
#include <iostream>
#include "Game.h"
#include "Actor.h"
#include "Renderer.h"
#include "Player.h"
#include "PlayerMove.h"

using namespace std;

HUDMaze::HUDMaze(Actor* parent): UIComponent(parent)
{
	this->mFont = new Font(parent->GetGame());
	this->mFont->Load("Assets/Font/HuaWenXiHei-1.ttf");

	this->mTimerText = mFont->RenderText("00:00.00", Color::White, 40, true);
	this->mSuccessText = mFont->RenderText("CompleteText", Color::Green, 40);
	this->mLoseText = mFont->RenderText("TimeUpText", Color::DarkRed, 40);

	this->mPos = Vector2(0.0f, 100.0f);

	this->mTimer = 10.0f;
}


HUDMaze::~HUDMaze()
{
	this->mFont->Unload();
	delete mFont;
}


void HUDMaze::Update(float deltaTime)
{
	mTimer -= deltaTime;

	if(mTimer <= 0.0f)
	{
		mOwner->GetGame()->player->GetComponent<PlayerMove>()->Finish();
		showText = -1;
	}
	else
	{
		float timer = mTimer;

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

		mTimerText = mFont->RenderText(time, Color::White, 40, true);
	}
}


void HUDMaze::Draw(class Shader* shader)
{
	if(showText == 0)
	{
		DrawTexture(shader, mTimerText, mPos);
	}
	else if(showText == -1)
	{
		DrawTexture(shader, mLoseText, mPos);
	}
	else if(showText == 1)
	{
		DrawTexture(shader, mSuccessText, mPos);
	}
}






