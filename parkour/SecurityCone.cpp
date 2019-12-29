#include "SecurityCone.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Player.h"
#include "PlayerMove.h"
#include "CollisionComponent.h"
#include <iostream>

using namespace std;

SecurityCone::SecurityCone(Game* game, Actor* parent):Actor(game, parent)
{
	this->SecurityCone_mesh = new MeshComponent(this, true); 
	this->SecurityCone_mesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cone.gpmesh"));

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
    if(angle <= (Math::Pi)/6.0f && length <= 300.0f && !InYellow && !InRed)
	{
		sound = Mix_PlayChannel(Mix_GroupAvailable(1), GetGame()->GetSound("Assets/Sounds/SecurityDetected.wav"), 0);

		InYellow = true;
		caught = true;
		YellowTime = 0.0f;
		SecurityCone_mesh->SetTextureIndex(1);
	}
// change into white
	else if((angle>(Math::Pi)/6.0f || length>300.0f) && InYellow && !InRed)
	{
		InYellow = false;
		YellowTime = 0.0f;

		caught = false;

		if(sound != -1)
		{
			Mix_HaltChannel(sound);
			sound = -1;
		}
		SecurityCone_mesh->SetTextureIndex(0);
	}

	if(InYellow)
	{
		YellowTime += deltaTime;

		if(YellowTime > KeepingYellow)
		{
            InRed = true;
            RedTime = 0.0f;

			InYellow = false;
			YellowTime = 0.0f;

			SecurityCone_mesh->SetTextureIndex(2);
		}
	}

	if(InRed)
	{
		RedTime += deltaTime;

		if(RedTime > KeepingRed)
		{
            InRed = false;
            caught = false;
			GetGame()->player->GetComponent<PlayerMove>()->Respawn();
			SecurityCone_mesh->SetTextureIndex(0);
		}
	}
}






