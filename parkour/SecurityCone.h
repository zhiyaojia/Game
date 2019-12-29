#pragma once
#include "Actor.h"
#include "Math.h"

class SecurityCone: public Actor
{
public:
	SecurityCone(class Game* game, Actor* parent);

	void OnUpdate(float deltaTime) override;

	bool CatchPlayer(){return caught;}

private:
	class MeshComponent* SecurityCone_mesh;

	const float KeepingYellow = 2.0f;
	const float KeepingRed = 0.5f;

	float YellowTime = 0.0f;
	float RedTime = 0.0f;

	bool InYellow = false;
	bool InRed = false;
	bool caught = false;

	int sound;
};
