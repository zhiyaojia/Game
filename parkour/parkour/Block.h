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

	void SetTurnOver(int t) {mTurnOver = t;}
	int GetTurnOver() {return mTurnOver;}

	void SetLast(bool last) {mLast = last;}
	bool GetLast() {return mLast;}

    void SetStopMove(bool stop) {mStopMove = stop;}
	bool GetStopMove() {return mStopMove;}

	void OnUpdate(float deltatime) override;

private:
	class MeshComponent* Block_mesh;
	class CollisionComponent* Block_collision;

	bool mRotate = false;
	bool mLast = false;

	bool mStopMove = false;

	int mTurnOver = 0;
};
