#include "SecurityCamera.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Player.h"
#include "CollisionComponent.h"
#include "SecurityCone.h"
#include <iostream>

using namespace std;

SecurityCamera::SecurityCamera(Game* game, Actor* parent):Actor(game, parent)
{
	this->SecurityCamera_mesh = new MeshComponent(this); 
	this->SecurityCamera_mesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/GameObject/Camera.gpmesh"));

	GetGame()->addToCameraArray(this);

	this->cone = new SecurityCone(game, this);
}


SecurityCamera::~SecurityCamera()
{
	GetGame()->removeFromCameraArray(this);
}


void SecurityCamera::OnUpdate(float deltaTime)
{
	if(running < 2) {return;}

	if(cone->CatchPlayer()) {return;}	

	timer += deltaTime;

	if(mState == SecurityCamera::Forward)
	{
		if(timer > interpTime)
		{
			mQuaternion = endQ;
			ChangeState(SecurityCamera::FrontEnd);
		}
		else
		{
			mQuaternion = Quaternion::Slerp(startQ, endQ, timer/interpTime);
		}
	}
	else if(mState == SecurityCamera::Backward)
	{
		if(timer > interpTime)
		{
			mQuaternion = startQ;
			ChangeState(SecurityCamera::BackEnd);
		}
		else
		{
			mQuaternion = Quaternion::Slerp(endQ, startQ, timer/interpTime);
		}
	}
	else if(mState == SecurityCamera::FrontEnd)
	{
		if(timer > pauseTime)
		{
			ChangeState(SecurityCamera::Backward);
		}
	}
	else if(mState == SecurityCamera::BackEnd)
	{
		if(timer > pauseTime)
		{
			ChangeState(SecurityCamera::Forward);
		}
	}
	
}


void SecurityCamera::SetParameter(Quaternion start, Quaternion end, float interpTime, float pause)
{
	this->startQ = start; 
	this->endQ = end;
	this->interpTime = interpTime;
	this->pauseTime = pause;

	mQuaternion = startQ;

	running = 2;
}


void SecurityCamera::ChangeState(MoveState m)
{
	mState = m;
	timer = 0.0f;
}









