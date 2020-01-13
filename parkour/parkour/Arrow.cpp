#include "Arrow.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Player.h"
#include "Checkpoint.h"
#include <iostream>
#include <queue>

using namespace std;

Arrow::Arrow(Game* game, Actor* parent):Actor(game, parent)
{
	this->Arrow_mesh = new MeshComponent(this); 
	this->Arrow_mesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/GameObject/Arrow.gpmesh"));

	SetScale(0.15f);
}

void Arrow::OnUpdate(float deltaTime)
{
	Quaternion RotationQuaternion;
	
	if(GetGame()->CheckpointArray.empty() || 
		GetGame()->CheckpointArray.front() == NULL || 
		GetGame()->GetGameMode() == Game::Build )
	{
		RotationQuaternion = Quaternion::Identity;
	}
	else
	{
		Vector3 forward = GetGame()->CheckpointArray.front()->GetPosition() - GetGame()->player->GetPosition();
		forward.Normalize();
	    
		float dot = Vector3::Dot(Vector3::UnitX, forward);
		
		if(Math::NearZero(1 - dot))
		{
			RotationQuaternion = Quaternion::Identity;
		}
		else if(Math::NearZero(1 + dot))
		{
			RotationQuaternion = Quaternion(Vector3::UnitZ, Math::Pi);
		}
		else
		{
			float angle = Math::Acos(dot);
			Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
			axis.Normalize();

			RotationQuaternion = Quaternion(axis, angle);
		}
	}

	mQuaternion = RotationQuaternion;

	Vector3 pos = GetGame()->GetRenderer()->Unproject(Vector3(0.0f, 250.0f, 0.1f));
	SetPosition(pos);
}






