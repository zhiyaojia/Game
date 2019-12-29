#pragma once
#include "Actor.h"
#include <vector>
#include "Math.h"

class Arrow: public Actor
{
public:
	Arrow(class Game* game, Actor* parent);

	void OnUpdate(float deltaTime) override;

private:
	class MeshComponent* Arrow_mesh;
};
