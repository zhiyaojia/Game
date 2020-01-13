#include "HUDTutorial.h"
#include "Texture.h"
#include "Font.h"
#include <string>
#include "Game.h"
#include "Actor.h"
#include "Renderer.h"
#include "Player.h"
#include "PlayerMove.h"
#include "SecurityCamera.h"
#include <iostream>
#include <SDL/SDL_ttf.h>

using namespace std;

const Vector2 radarPos = Vector2(410.0f, -285.0f);
const float radarRange = 1500.0f;
const float radarRadius = 92.0f;

HUDTutorial::HUDTutorial(Actor* parent): UIComponent(parent)
{
	this->mFont = new Font(parent->GetGame());
	this->mFont->Load("Assets/Font/HuaWenXiHei-1.ttf");

	this->mHeartTextPos = Vector2(-380.0f, -250.0f);
	this->mHeartImagePos = Vector2(-450.0f, -250.0f);
	this->mHeartText = mFont->RenderText("x 0", Color::DarkRed, 32, true);
	this->mHeartImgae = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/image/heart.png");

	this->mBackgroundTexture = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/image/Radar.png");
	this->mArrowTexture = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/image/RadarArrow.png");
	this->mBlipTexture = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/image/Blip.png");

	this->mCheckpointTexture = nullptr;
}


HUDTutorial::~HUDTutorial()
{
	this->mFont->Unload();
	delete mFont;
}


void HUDTutorial::Update(float deltaTime)
{
// Update Life
	mHeartText->Unload();
	delete mHeartText;
	mHeartText = mFont->RenderText("x " + to_string(
				mOwner->GetGame()->player->GetComponent<PlayerMove>()->GetLife()), 
				Color::DarkRed, 32, true);

// Update Checkpoint Text
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


void HUDTutorial::Draw(class Shader* shader)
{
	if(mCheckpointTimer <= 5.0f && mCheckpointTexture != NULL)
	{
		DrawTexture(shader, mCheckpointTexture, Vector2(0, 0));
	}

	DrawTexture(shader, mHeartText, mHeartTextPos);
	DrawTexture(shader, mHeartImgae, mHeartImagePos);

	DrawTexture(shader, mBackgroundTexture, radarPos);
	DrawTexture(shader, mArrowTexture, radarPos);

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


void HUDTutorial::updateCheckpointText(string text)
{
	mCheckpointTimer = 5.0f;
	mCheckpointTexture = mFont->RenderText(text, Color::White, 30);
}








