#pragma once
#include "Actor.h"
#include "Math.h"

class Heart: public Actor
{
public:
	Heart(class Game* game, Actor* parent);

	void OnUpdate(float deltaTime) override;

private:
	class MeshComponent* Heart_mesh;
	class CollisionComponent* Heart_collision;
};
