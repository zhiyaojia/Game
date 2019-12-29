#include "Player.h"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "CameraComponent.h"
#include "PlayerMove.h"
#include "Renderer.h"
#include "HUD.h"
#include <iostream>

using namespace std;

Player::Player(Game* game, Actor* parent):Actor(game, parent)
{
	this->Player_move = new PlayerMove(this);

	this->Player_camera = new CameraComponent(this);

	this->Player_collision = new CollisionComponent(this);
	this->Player_collision->SetSize(50.0f, 250.0f, 50.0f);

	this->playerHUD = new HUD(this);
}



















