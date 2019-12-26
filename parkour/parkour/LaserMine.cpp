#include "LaserMine.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include <iostream>

using namespace std;

LaserMine::LaserMine(Game* game, Actor* parent):Actor(game, parent)
{
	this->LaserMine_mesh = new MeshComponent(this); 
	this->LaserMine_mesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/LaserMine.gpmesh"));
}

















