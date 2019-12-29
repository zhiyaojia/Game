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

	float GetPitchSpeed() {return mPitchSpeed;}
	float GetPitchAngle() {return mPitchAngle;}

	void SetPitchSpeed(float speed) {mPitchSpeed = speed;}

	void SetUp(float x, float y) {Up.x = x; Up.y = y;}

	void SetAngleLimit(float angle) {angle_limit = angle;}

	void SetRotationSpeed(float speed) {RotationSpeed = speed;}

	void UpdateAngle(float deltaTime, Vector2 Multiplier);

	void FixUp();
    void Fixing(float deltaTime);

private:
	Vector3 GetIdeadPos();

	float HDist = 60.0f;
    float TargetDist = 50.0f;
    float mPitchAngle, mPitchSpeed;
    float angle_limit = Math::PiOver2 / 2.0f;
    float RotationSpeed = 0.0f;

    Vector3 CameraPos, CameraVelocity;
    Vector3 Up = Vector3::UnitZ;
    
    bool fixing = false;
};









