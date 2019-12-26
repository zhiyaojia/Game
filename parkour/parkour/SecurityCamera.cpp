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
	this->SecurityCamera_mesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Camera.gpmesh"));

	sound = Mix_PlayChannel(Mix_GroupAvailable(1), GetGame()->GetSound("Assets/Sounds/CameraMotor.wav"), 0);
	Mix_Volume(sound, 0);

	GetGame()->addToCameraArray(this);

	this->cone = new SecurityCone(game, this);
}


SecurityCamera::~SecurityCamera()
{
	if(sound != -1)
	{
		Mix_HaltChannel(sound);
	}

	GetGame()->removeFromCameraArray(this);
}


void SecurityCamera::OnUpdate(float deltaTime)
{
	if(this->cone->CatchPlayer())
	{
		if(!catchMusic)
		{
			HaltMusic();
		}
		catchMusic = true;
		return;
	}
	else if(catchMusic && (mState == MoveState::Forward || mState == MoveState::Backward))
	{
		catchMusic = false;
		sound = Mix_PlayChannel(Mix_GroupAvailable(1), GetGame()->GetSound("Assets/Sounds/CameraMotor.wav"), 0);
	}

	if(sound != -1)
	{
		FixVolume();
	}

	timer += deltaTime;

	if(mState == SecurityCamera::Forward)
	{
		mQuaternion = Quaternion::Slerp(startQ, endQ, timer/interpTime);

		if(timer > interpTime)
		{
			mQuaternion = endQ;
			ChangeState(SecurityCamera::FrontEnd);
			HaltMusic();
		}
	}
	else if(mState == SecurityCamera::Backward)
	{
		mQuaternion = Quaternion::Slerp(endQ, startQ, timer/interpTime);

		if(timer > interpTime)
		{
			mQuaternion = startQ;
			ChangeState(SecurityCamera::BackEnd);
			HaltMusic();
		}
	}
	else if(mState == SecurityCamera::FrontEnd)
	{
		if(timer > pauseTime)
		{
			ChangeState(SecurityCamera::Backward);
			sound = Mix_PlayChannel(Mix_GroupAvailable(1), GetGame()->GetSound("Assets/Sounds/CameraMotor.wav"), 0);
		}
	}
	else if(mState == SecurityCamera::BackEnd)
	{
		if(timer > pauseTime)
		{
			ChangeState(SecurityCamera::Forward);
			sound = Mix_PlayChannel(Mix_GroupAvailable(1), GetGame()->GetSound("Assets/Sounds/CameraMotor.wav"), 0);
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
}


void SecurityCamera::ChangeState(MoveState m)
{
	mState = m;
	timer = 0.0f;
}


void SecurityCamera::HaltMusic()
{
	if(sound != -1)
	{
		Mix_HaltChannel(sound);
	}
	Mix_PlayChannel(sound, GetGame()->GetSound("Assets/Sounds/CameraMotorStop.wav"), 0);
	sound = -1;
}


void SecurityCamera::FixVolume()
{
	Vector3 dist = GetGame()->player->GetPosition() - GetPosition();
	float length = dist.Length();
	
	if(length > 1500.0f)
	{
		Mix_Volume(sound, 0);
	}
	else if(length < 500.0f)
	{
		Mix_Volume(sound, 128);
	}
	else
	{
		float value = (1500.0f - length)/1000.0f;
		float volume = Math::Lerp(0.0f, 128.0f, value);
		Mix_Volume(sound, static_cast<int>(volume));
	}
}








