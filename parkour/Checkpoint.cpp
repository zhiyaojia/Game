#include "Checkpoint.h"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Renderer.h"
#include "Player.h"
#include <iostream>
#include <queue>

using namespace std;

Checkpoint::Checkpoint(Game* game, Actor* parent):Actor(game, parent)
{
	this->Checkpoint_mesh = new MeshComponent(this); 
	this->Checkpoint_mesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Checkpoint.gpmesh"));

	this->Checkpoint_collision = new CollisionComponent(this);
	this->Checkpoint_collision->SetSize(25.0f, 25.0f, 25.0f);
}


void Checkpoint::OnUpdate(float deltaTime)
{
	if(active)
	{
		this->Checkpoint_mesh->SetTextureIndex(0);
	}
	else
	{
		this->Checkpoint_mesh->SetTextureIndex(1);
		return;
	}
}


void Checkpoint::Active()
{
	active = true;
}

void Checkpoint::Deactive()
{
	active = false;
    SetState(ActorState::Destroy);

    if(!mLevelString.empty())
    {
    	GetGame()->SetLevel(mLevelString);
    }
}











