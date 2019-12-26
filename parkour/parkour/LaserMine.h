#pragma once
#include "Actor.h"
#include <vector>

class LaserMine: public Actor
{
public:
	LaserMine(class Game* game, Actor* parent);

private:
	class MeshComponent* LaserMine_mesh;
};
