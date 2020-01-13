#pragma once
#include "Actor.h"
#include <vector>

class LaserMine: public Actor
{
public:
	LaserMine(class Game* game, Actor* parent);

	float GetLaserLength() {return mLength;}
    void SetLaserLength(float length) {mLength = length;}

	bool GetLaserRandom() {return mRandom;}
	void SetLaserRandom(bool r) {mRandom = r;}

private:
	class MeshComponent* LaserMine_mesh;

	float mLength = 500.0f;

	bool mRandom = false;
};
