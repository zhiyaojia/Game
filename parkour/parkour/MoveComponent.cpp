#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner)
:Component(owner, 50)
,mAngularSpeed(0.0f)
,mForwardSpeed(0.0f)
,mStrafeSpeed(0.0f)
{
	
}

void MoveComponent::Update(float deltaTime)
{

}
