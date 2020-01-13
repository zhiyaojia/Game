#include "Player.h"
#include "Game.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "CameraComponent.h"
#include "PlayerMoveBuild.h"
#include "PlayerMoveAdventure.h"
#include "PlayerMove.h"
#include "Renderer.h"
#include "UIComponent.h"
#include "HUDAdventure.h"
#include "HUDBuild.h"
#include "HUDMaze.h"
#include "HUDTutorial.h"
#include "HUDPractice.h"
#include <iostream>

using namespace std;

Player::Player(Game* game, Actor* parent):Actor(game, parent)
{
	this->Player_camera = new CameraComponent(this);

	this->Player_collision = new CollisionComponent(this);
	this->Player_collision->SetSize(50.0f, 250.0f, 50.0f);

	if(game->GetGameMode() == Game::Adventure)
	{	
		this->mAdventureMove = new PlayerMoveAdventure(this);
		this->mAdventrueHUD = new HUDAdventure(this);
	}
	else if(game->GetGameMode() == Game::Build)
	{
		this->mBuildHUD = new HUDBuild(this);
		this->mBuildMove = new PlayerMoveBuild(this);
	}
	else if(game->GetGameMode() == Game::Maze)
	{
		this->mPlayerMove = new PlayerMove(this, "Maze");
		this->mMazeHUD = new HUDMaze(this);
	}
	else if(game->GetGameMode() == Game::Practice)
	{
		this->mPlayerMove = new PlayerMove(this, "Practice");
		this->mPracticeHUD = new HUDPractice(this);
	}
	else if(game->GetGameMode() == Game::Tutorial)
	{
		this->mPlayerMove = new PlayerMove(this, "Tutorial");
		this->mTutorialHUD = new HUDTutorial(this);
	}
}



















