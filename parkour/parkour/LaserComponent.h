#pragma once
#include "Math.h"
#include "MeshComponent.h"
#include "SegmentCast.h"
#include <vector>

class LaserComponent : public MeshComponent
{
public:
	LaserComponent(class Actor* owner, bool random=false, float length=500.0f);
	
	void Update(float deltaTime) override;

	void Draw(class Shader* shader) override;

	void SetLength(float length) {mLength = length;}
	float GetLenght() {return mLength;}

	Matrix4 Transform(struct LineSegment Laser);

private:
	std::vector<struct LineSegment> LaserArray;

	float mLength;

	bool mActive = true;
	bool mRandom = true;

	bool onStatus = false;
	float timer = 0.0f;
	float mStatusTime = 1.0f;
};
