#include "CameraComponent.h"
#include "Game.h"
#include "Actor.h"
#include "Renderer.h"
#include <iostream>

using namespace std;

CameraComponent::CameraComponent(Actor* owner): Component(owner)
{
	this->CameraPos = Vector3::Zero;
	this->CameraVelocity = Vector3::Zero;
	this->mPitchAngle = 0.0f;
	this->mPitchSpeed = 0.0f;
}

void CameraComponent::Update(float deltaTime)
{
// Update pitch angle
	mPitchAngle += mPitchSpeed * deltaTime;
	if(mPitchAngle < -angle_limit)
		mPitchAngle = -angle_limit;
	if(mPitchAngle > angle_limit)
		mPitchAngle = angle_limit;

	Matrix4 pitch = Matrix4::CreateRotationY(mPitchAngle);
	Matrix4 yaw = Matrix4::CreateRotationZ(mOwner->GetRotation());
	Matrix4 combine = pitch * yaw;
	Vector3 forward = Vector3::Transform(Vector3(1, 0, 0), combine);  
	Vector3 TargetPos = mOwner->GetPosition() + (forward * TargetDist);

	Matrix4 view = Matrix4::CreateLookAt(mOwner->GetPosition(), TargetPos, Up);
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(view);

// Update Up
	if(fixing)
	{
		Fixing(deltaTime);
	}
}


Vector3 CameraComponent::GetIdeadPos()
{
	Vector3 IdealPos = mOwner->GetPosition() - (mOwner->GetForward() * HDist) + (Vector3::UnitZ * 70.0f);
	return IdealPos;
}


void CameraComponent::UpdateAngle(float deltaTime, Vector2 Multiplier)
{
	Up.x += deltaTime * RotationSpeed * Multiplier.x;
	Up.y += deltaTime * RotationSpeed * Multiplier.y;
}


void CameraComponent::FixUp()
{
	fixing = true;
}


void CameraComponent::Fixing(float deltaTime)
{
	if(Up.y < 0.0f)
	{
		Up.y += deltaTime * RotationSpeed;
		if(Up.y >= 0.0f)
		{
			fixing = false;
			Up.y = 0.0f;
		}
	}
	else if(Up.y > 0.0f)
	{
		Up.y -= deltaTime * RotationSpeed;
		if(Up.y <= 0.0f)
		{
			fixing = false;
			Up.y = 0.0f;
		}
	}

	if(Up.x < 0.0f)
	{
		Up.x += deltaTime * RotationSpeed;
		if(Up.x >= 0.0f)
		{
			fixing = false;
			Up.x = 0.0f;
		}
	}
	else if(Up.x > 0.0f)
	{
		Up.x -= deltaTime * RotationSpeed;
		if(Up.x <= 0.0f)
		{
			fixing = false;
			Up.x = 0.0f;
		}
	}
}




















