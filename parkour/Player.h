#pragma once
#include "Actor.h"
#include "Math.h"

class Player: public Actor
{
public:
	Player(class Game* game, Actor* parent);

	Vector3 GetRespawn(){return respawn;}
	void SetRespawn(Vector3 r){respawn = r;}

private:
	class PlayerMove* Player_move;
	class CameraComponent* Player_camera;
	class CollisionComponent* Player_collision;

	Vector3 respawn;

	class HUD* playerHUD;
};
