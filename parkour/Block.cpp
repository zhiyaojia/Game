#include "Block.h"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Renderer.h"
#include <iostream>

using namespace std;

Block::Block(Game* game, Actor* parent):Actor(game, parent)
{
	this->mScale = 64.0f;

	this->Block_mesh = new MeshComponent(this); 
	this->Block_mesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));

	this->Block_collision = new CollisionComponent(this);
	this->Block_collision->SetSize(1.0f, 1.0f, 1.0f);

	GetGame()->Object.push_back(this);
}


Block::~Block()
{
    vector<Actor*>::iterator it = find(GetGame()->Object.begin(), 
    	GetGame()->Object.end(), this);
	if(it != GetGame()->Object.end())
		GetGame()->Object.erase(it);
}


void Block::OnUpdate(float deltatime)
{
	if(mRotate)
	{
		float angle = GetRotation();
		angle += deltatime * (Math::PiOver2 / 2.0f);
		SetRotation(angle);
	}
}




















