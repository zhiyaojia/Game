#include "Heart.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Player.h"
#include "PlayerMoveAdventure.h"
#include "PlayerMove.h"
#include "CollisionComponent.h"
#include "HUDAdventure.h"
#include <iostream>
#include <queue>

using namespace std;

Heart::Heart(Game* game, Actor* parent):Actor(game, parent)
{
	this->Heart_mesh = new MeshComponent(this); 
	this->Heart_mesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/GameObject/Heart.gpmesh"));

	this->Heart_collision = new CollisionComponent(this);
	this->Heart_collision->SetSize(100.0f, 100.0f, 100.0f);
}

void Heart::OnUpdate(float deltaTime)
{
	float angle = GetRotation();
    angle += deltaTime * (Math::Pi);
	SetRotation(angle);

	if(mGame->GetGameMode() == Game::Build) {return;}

    CollisionComponent* player_collision = GetGame()->player->GetComponent<CollisionComponent>();
	
	if(Heart_collision->Intersect(player_collision))
	{
		Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Coin.wav"), 0);
		SetState(ActorState::Destroy);

		if(mGame->GetGameMode() == Game::Adventure)
		{
			mGame->player->GetComponent<PlayerMoveAdventure>()->AddLife();
		}
		else if(mGame->GetGameMode() == Game::Tutorial)
		{
			mGame->player->GetComponent<PlayerMove>()->AddLife();
		}
	}
}






