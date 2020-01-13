#pragma once
#include "Actor.h"
#include "Math.h"

class SecurityCone: public Actor
{
public:
	SecurityCone(class Game* game, Actor* parent);

	void OnUpdate(float deltaTime) override;

	bool CatchPlayer() {return caught;}

private:
	class MeshComponent* SecurityCone_mesh;

	const float KeepingRed = 1.0f;

	float RedTime = 0.0f;

	bool InRed = false;
	bool caught = false;
};
