#include "Coin.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Player.h"
#include "CollisionComponent.h"
#include "HUDAdventure.h"
#include <iostream>
#include <queue>

using namespace std;

Coin::Coin(Game* game, Actor* parent):Actor(game, parent)
{
	this->Coin_mesh = new MeshComponent(this); 
	this->Coin_mesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/GameObject/Coin.gpmesh"));

	this->Coin_collision = new CollisionComponent(this);
	this->Coin_collision->SetSize(100.0f, 100.0f, 100.0f);
}

void Coin::OnUpdate(float deltaTime)
{
	float angle = GetRotation();
    angle += deltaTime * (Math::Pi);
	SetRotation(angle);

	if(mGame->GetGameMode() == Game::Build) {return;}

    CollisionComponent* player_collision = GetGame()->player->GetComponent<CollisionComponent>();
	
	if(Coin_collision->Intersect(player_collision))
	{
		mGame->addCoin();
		Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Coin.wav"), 0);
		SetState(ActorState::Destroy);
	}
}






