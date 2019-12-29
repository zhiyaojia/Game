#pragma once
#include "Actor.h"
#include "Math.h"

class Coin: public Actor
{
public:
	Coin(class Game* game, Actor* parent);

	void OnUpdate(float deltaTime) override;

private:
	class MeshComponent* Coin_mesh;
	class CollisionComponent* Coin_collision;
};
