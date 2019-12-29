#pragma once
#include "Component.h"
#include "Math.h"

enum class CollSide
{
	None,
	Top,
	Bottom,
	SideX1,
	SideX2,
	SideY1,
	SideY2
};

class CollisionComponent : public Component
{
public:
	CollisionComponent(class Actor* owner);
	~CollisionComponent();

	// Set width/height of this box
	void SetSize(float width, float height, float depth)
	{
		mWidth = width;
		mHeight = height;
		mDepth = depth;
	}

	// Returns true if this box intersects with other
	bool Intersect(const CollisionComponent* other);

	// Get min and max points of box
	Vector3 GetMin() const;
	Vector3 GetMax() const;

	// Get width, height, center of box
	const Vector3& GetCenter() const;
	float GetWidth() const { return mWidth; }
	float GetHeight() const { return mHeight; }
	float GetDepth() const { return mDepth; }

	static bool IsSide(CollSide side)
	{
        bool x = (side == CollSide::SideX1 || side == CollSide::SideX2 
        	|| side == CollSide::SideY1 || side == CollSide::SideY2);
		return x;
	}

	
	// Returns side of minimum overlap against other
	// or None if no overlap
	// Takes in by reference the offset to fix
	// "this" so it no longer overlaps with "other"
	CollSide GetMinOverlap(const CollisionComponent* other, Vector3& offset);
private:
	float mWidth;
	float mHeight;
	float mDepth;
};
