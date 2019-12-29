#pragma once
#include "Actor.h"
#include "Math.h"

class SecurityCamera: public Actor
{
public:
	enum MoveState
	{
		Forward,
		Backward,
		FrontEnd,
		BackEnd
	};

	SecurityCamera(class Game* game, Actor* parent);
	~SecurityCamera();

	void OnUpdate(float deltaTime) override;

	void SetParameter(Quaternion start, Quaternion end, float interpTime, float pause);

private:
	class MeshComponent* SecurityCamera_mesh;
	class SecurityCone* cone;

	Quaternion startQ, endQ;
	
	float interpTime, pauseTime;
	float timer = 0.0f;

	MoveState mState = Forward;

	void ChangeState(MoveState m);

	void HaltMusic();
	void FixVolume();

	int sound;

	bool catchMusic = false;
};