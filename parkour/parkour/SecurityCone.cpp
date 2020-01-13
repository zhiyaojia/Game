#include "SecurityCone.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Player.h"
#include "PlayerMoveAdventure.h"
#include "PlayerMove.h"
#include "CollisionComponent.h"
#include "SecurityCamera.h"
#include <iostream>

using namespace std;

SecurityCone::SecurityCone(Game* game, Actor* parent):Actor(game, parent)
{
	this->SecurityCone_mesh = new MeshComponent(this, true); 
	this->SecurityCone_mesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/GameObject/Cone.gpmesh"));

	SetPosition(Vector3(30, 0, 0));
	SetScale(3.0f);
}

void SecurityCone::OnUpdate(float deltaTime)
{
	Vector3 dist = GetGame()->player->GetWorldPosition() - GetWorldPosition();
	float length = dist.Length();

	dist.Normalize();

	float angle = Math::Acos(Vector3::Dot(dist, GetWorldForward()));

// change into yellow
	if(angle <= (Math::Pi)/6.0f && length <= 300.0f && !InRed)
	{
		if(mGame->GetSoundOn())
		{
			Mix_PlayChannel(mGame->mCameraCatchSound, GetGame()->GetSound("Assets/Sounds/CameraCatch.wav"), 0);
		}

		InRed = true;
		caught = true;
		RedTime = 0.0f;
		SecurityCone_mesh->SetTextureIndex(1);
	}
// change into white
	else if((angle>(Math::Pi)/6.0f || length>300.0f) && InRed)
	{
		InRed = false;
		RedTime = 0.0f;

		caught = false;

		SecurityCone_mesh->SetTextureIndex(0);
	}

	if(InRed)
	{
		RedTime += deltaTime;

        Game::GameMode g = GetGame()->GetGameMode();

		if(g == Game::Adventure)
		{
			GetGame()->player->GetComponent<PlayerMoveAdventure>()->GetDetect();
		}
		else if(g == Game::Tutorial || g == Game::Practice)
		{
			GetGame()->player->GetComponent<PlayerMove>()->GetDetect();
		}

		if(RedTime > KeepingRed)
		{
			InRed = false;
			caught = false;

			if(g == Game::Adventure)
			{
				GetGame()->player->GetComponent<PlayerMoveAdventure>()->GetCaught();
			}
			else if(g == Game::Tutorial || g == Game::Practice)
			{
				GetGame()->player->GetComponent<PlayerMove>()->GetCaught();
			}
			
			SecurityCone_mesh->SetTextureIndex(0);
		}
	}
}






