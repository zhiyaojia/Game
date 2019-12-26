#pragma once
#include "Math.h"
#include "MeshComponent.h"
#include "SegmentCast.h"
#include <vector>

class LaserComponent : public MeshComponent
{
public:
	LaserComponent(class Actor* owner);
	
	void Update(float deltaTime) override;

	void Draw(class Shader* shader) override;

	Matrix4 Transform(struct LineSegment Laser);

private:
	std::vector<struct LineSegment> LaserArray;
};
