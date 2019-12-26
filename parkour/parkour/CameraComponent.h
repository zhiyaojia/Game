#pragma once
#include "Component.h"
#include "Math.h"
#include <string>
#include <vector>

class CameraComponent: public Component
{
public:
	CameraComponent(class Actor* owner);

	void Update(float deltaTime) override;

	void SetPitchSpeed(float speed) {mPitchSpeed = speed;}
	float GetPitchSpeed() {return mPitchSpeed;}
	float GetPitchAngle() {return mPitchAngle;}

	void SetAngleLimit(float angle) {angle_limit = angle;}

	void UpdateAngle(float deltaTime, Vector2 Multiplier);

	void FixUp();

private:
	Vector3 GetIdeadPos();

	float HDist = 60.0f;
    float TargetDist = 50.0f;
    float mPitchAngle, mPitchSpeed;
    float angle_limit = Math::PiOver2 / 2.0f;
    const float RotationSpeed = 0.5f;

    Vector3 CameraPos, CameraVelocity;
    Vector3 Up = Vector3::UnitZ;

    bool rotate = false;
};









