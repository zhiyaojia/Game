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

	void SetStartQ() {startQ = GetQuaternion(); running++;}
	void SetEndQ() {endQ = GetQuaternion(); running++;}
	Quaternion GetStartQ() {return startQ;}
	Quaternion GetEndQ() {return endQ;}
	float GetInterpTime() {return interpTime;}
	float GetPauseTime() {return pauseTime;}

private:
	class MeshComponent* SecurityCamera_mesh;
	class SecurityCone* cone;

	Quaternion startQ, endQ;
	
	float interpTime = 4.0f;
	float pauseTime = 2.0f;
	float timer = 0.0f;

	MoveState mState = Forward;

	void ChangeState(MoveState m);

// used to build Camera in Build mode
	int running = 0;
};






