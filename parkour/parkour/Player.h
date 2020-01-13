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
	class CameraComponent* Player_camera;
	class CollisionComponent* Player_collision;

	Vector3 respawn;

	class PlayerMoveBuild* mBuildMove = nullptr;
	class PlayerMoveAdventure* mAdventureMove = nullptr;
	class PlayerMove* mPlayerMove = nullptr;

	class HUDBuild* mBuildHUD = nullptr;
	class HUDAdventure* mAdventrueHUD = nullptr;
	class HUDPractice* mPracticeHUD = nullptr;
	class HUDTutorial* mTutorialHUD = nullptr;
	class HUDMaze* mMazeHUD = nullptr;
};	
