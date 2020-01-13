#include "HUDPractice.h"
#include "Texture.h"
#include <SDL/SDL_ttf.h>
#include "Font.h"
#include <string>
#include <iostream>
#include "Game.h"
#include "Actor.h"
#include "Renderer.h"
#include "Player.h"

using namespace std;

HUDPractice::HUDPractice(Actor* parent): UIComponent(parent)
{
	this->mFont = new Font(parent->GetGame());
	this->mFont->Load("Assets/Font/HuaWenXiHei-1.ttf");

	this->mCheckpointTexture = nullptr;
}


HUDPractice::~HUDPractice()
{
	this->mFont->Unload();
	delete mFont;
}


void HUDPractice::Update(float deltaTime)
{
	if(mCheckpointTimer <= 5.0f && mCheckpointTexture != NULL)
	{
		mCheckpointTimer -= deltaTime;
		if(mCheckpointTimer < 0.0f)
		{
            mCheckpointTexture->Unload();
			delete mCheckpointTexture;
			mCheckpointTexture = nullptr;
		}
	}
}


void HUDPractice::Draw(class Shader* shader)
{
	if(mCheckpointTimer <= 5.0f && mCheckpointTexture != NULL)
	{
		DrawTexture(shader, mCheckpointTexture, Vector2(0, 0));
	}
}


void HUDPractice::updateCheckpointText(string text)
{
	mCheckpointTimer = 5.0f;
	mCheckpointTexture = mFont->RenderText(text, Color::DarkRed, 30);
}






