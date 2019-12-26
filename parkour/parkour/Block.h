#pragma once
#include "Actor.h"
#include <vector>

class Block: public Actor
{
public:
	Block(class Game* game, Actor* parent);

	~Block();

	void SetRotate(bool rotate){mRotate = rotate;}
	bool GetRotate(){return mRotate;}

	void OnUpdate(float deltatime) override;

private:
	class MeshComponent* Block_mesh;
	class CollisionComponent* Block_collision;

	bool mRotate = false;
};
