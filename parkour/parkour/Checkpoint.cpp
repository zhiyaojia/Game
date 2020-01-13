#include "Checkpoint.h"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Renderer.h"
#include "Player.h"
#include "HUDPractice.h"
#include <iostream>
#include <queue>

using namespace std;

Checkpoint::Checkpoint(Game* game, Actor* parent):Actor(game, parent)
{
	if(game->GetGameMode() != Game::Adventure)
	{
		this->Checkpoint_mesh = new MeshComponent(this); 
		this->Checkpoint_mesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/GameObject/Checkpoint.gpmesh"));

		if(game->GetGameMode() == Game::Build || mGame->GetGameMode() == Game::Practice)
		{
			active = true;
		}
	}

	this->Checkpoint_collision = new CollisionComponent(this);
	this->Checkpoint_collision->SetSize(25.0f, 25.0f, 25.0f);
}


void Checkpoint::OnUpdate(float deltaTime)
{
	if(mGame->GetGameMode() == Game::Practice)
	{
		CollisionComponent* player_collision = mGame->player->GetComponent<CollisionComponent>();
		if(player_collision->Intersect(Checkpoint_collision))
		{
			mGame->player->GetComponent<HUDPractice>()->updateCheckpointText(text);
		}
	}
	else if(mGame->GetGameMode() == Game::Tutorial)
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
}


void Checkpoint::Active()
{
	active = true;
}

void Checkpoint::Deactive()
{
	active = false;
    SetState(ActorState::Destroy);
}











