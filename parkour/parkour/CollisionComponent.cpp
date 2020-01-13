#include "CollisionComponent.h"
#include "Actor.h"
#include <algorithm>
#include <iostream>

using namespace std;

CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f)
,mDepth(0.0f)
{
	
}

CollisionComponent::~CollisionComponent()
{
	
}

bool CollisionComponent::Intersect(const CollisionComponent* other)
{
	Vector3 aMin = GetMin();
	Vector3 aMax = GetMax();
	Vector3 bMin = other->GetMin();
	Vector3 bMax = other->GetMax();

	bool no = aMax.x < bMin.x ||
		aMax.y < bMin.y ||
		aMax.z < bMin.z ||
		bMax.x < aMin.x ||
		bMax.y < aMin.y ||
		bMax.z < aMin.z;

	return !no;
}

Vector3 CollisionComponent::GetMin() const
{
	Vector3 v = mOwner->GetPosition();
	v.x -= mDepth * mOwner->GetScale() / 2.0f;
	v.y -= mWidth * mOwner->GetScale() / 2.0f;
	v.z -= mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

Vector3 CollisionComponent::GetMax() const
{
	Vector3 v = mOwner->GetPosition();
	v.x += mDepth * mOwner->GetScale() / 2.0f;
	v.y += mWidth * mOwner->GetScale() / 2.0f;
	v.z += mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

const Vector3& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(
	const CollisionComponent* other, Vector3& offset)
{
	bool collide = Intersect(other);
	if(!collide)
		return CollSide::None;
	
	float dx1 = abs(other->GetMin().x - GetMax().x);
	float dx2 = abs(other->GetMax().x - GetMin().x);
	float dy1 = abs(other->GetMin().y - GetMax().y);
	float dy2 = abs(other->GetMax().y - GetMin().y);
	float dz1 = abs(other->GetMin().z - GetMax().z);
	float dz2 = abs(other->GetMax().z - GetMin().z);

	vector<float> v = {dx1, dx2, dy1, dy2, dz1, dz2};

	float min = *min_element(v.begin(),v.end());

	if(min == dy1)
	{
		offset = Vector3(0, -dy1, 0);
		return CollSide::SideY1;
	}
	else if(min == dx1)
	{
		offset = Vector3(-dx1, 0, 0);
		return CollSide::SideX1;
	}
	else if(min == dx2)
	{
		offset = Vector3(dx2, 0, 0);
		return CollSide::SideX2;
	}
	else if(min == dy2)
	{
		offset = Vector3(0, dy2, 0);
		return CollSide::SideY2;
	}
	else if(min == dz1)
	{
		offset = Vector3(0, 0, -dz1);
		return CollSide::Bottom;
	}
	else if(min == dz2)
	{
		offset = Vector3(0, 0, dz2);
		return CollSide::Top;
	}

	return CollSide::None;
}



















