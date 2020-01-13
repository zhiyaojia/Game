#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include "Math.h"
#include <algorithm>
#include <iostream>

using namespace std;

Actor::Actor(Game* game, Actor* parent)
	:mGame(game)
	,mState(ActorState::Active)
	,mPosition(Vector3::Zero)
	,mScale(1.0f)
	,mRotation(0.0f)
	,mParent(parent)
{
	if(parent == NULL)
	{
		this->mGame->AddActor(this);
	}
	else
	{
		mParent->AddChild(this);
	}
}

Actor::~Actor()
{
	while (!mChildren.empty())
	{
		delete mChildren.back();
	}

	if(mParent == NULL)
	{
		this->mGame->RemoveActor(this);
	}
	else
	{
		mParent->RemoveChild(this);
	}

	for(auto c: this->mComponents)
		delete c;
	this->mComponents.clear();
}

void Actor::Update(float deltaTime)
{
	if(mRotation > Math::TwoPi)
	{
		mRotation -= Math::TwoPi;
	}
	else if(mRotation < 0.0f)
	{
		mRotation += Math::TwoPi;
	}

	CalcWorldTransform();

	if(this->mState == ActorState::Active)
	{
		for(auto c: this->mComponents)
		{
			if(c)
			{
				c->Update(deltaTime);
			}
		}
		OnUpdate(deltaTime);
	}

	CalcWorldTransform();

	for(auto child: mChildren)
	{
        if(child) {child->Update(deltaTime);}
	}

	UpdateMove(deltaTime);
	UpdateRotate(deltaTime);
}


void Actor::SetMovePath(Vector3 start, Vector3 end, float time)
{
	movable = 2;
	mStartPoint = start + mGame->GetFinalPos(); 
	mStartPoint.x += 500.0f;
	mEndPoint = end + mGame->GetFinalPos(); 
	mEndPoint.x += 500.0f;
	mMoveTime = time;
	mCurrentMoveTimer = 0.0f;
}


void Actor::SetStartPos()
{
	mStartPoint = mPosition; 
	movable++; 
	cout<<"Set mStartPoint: "; 
	mStartPoint.Print();
}


void Actor::SetEndPos()
{
	mEndPoint = mPosition; 
	movable++; 
	cout<<"Set mEndPoint: "; 
	mEndPoint.Print();
}


void Actor::UpdateMove(float deltaTime)
{
	if(movable < 2) {return;}

	mCurrentMoveTimer += deltaTime;
	if(mMoveDir == mForward)
	{
		if(mCurrentMoveTimer <= mMoveTime)
		{
			float thirdPara = mCurrentMoveTimer/mMoveTime;
			mPosition = Vector3::Lerp(mStartPoint, mEndPoint, thirdPara);
		}
		else
		{
			mCurrentMoveTimer = 0.0f;
			mPosition = mEndPoint;
			mMoveDir = mBackward;
		}
	}
	else if(mMoveDir == mBackward)
	{
		if(mCurrentMoveTimer <= mMoveTime)
		{
			float thirdPara = mCurrentMoveTimer/mMoveTime;
			mPosition = Vector3::Lerp(mEndPoint, mStartPoint, thirdPara);
		}
		else
		{
			mCurrentMoveTimer = 0.0f;
			mPosition = mStartPoint;
			mMoveDir = mForward;
		}
	}
}


void Actor::SetRotatePath(Quaternion start, Quaternion end, float time)
{
	rotable = 2;
	mStartQuat = start;
	mEndQuat = end;
	mRotateTime = time;
	mCurrentRotateTimer = 0.0f;
}


void Actor::SetStartQuat()
{
	mStartQuat = mQuaternion; 
	rotable++; 
	cout<<"Set mStartQuat: "; 
	mStartQuat.Print();
}


void Actor::SetEndQuat() 
{
	mEndQuat = mQuaternion; 
	rotable++; 
	cout<<"Set mEndQuat: "; 
	mEndQuat.Print();
}


void Actor::UpdateRotate(float deltaTime)
{
	if(rotable < 2) {return;}

	mCurrentRotateTimer += deltaTime;
	if(mRotateDir == rForward)
	{
		if(mCurrentRotateTimer <= mRotateTime)
		{
			float thirdPara = mCurrentRotateTimer/mRotateTime;
			mQuaternion = Quaternion::Slerp(mStartQuat, mEndQuat, thirdPara);
		}
		else
		{
			mCurrentRotateTimer = 0.0f;
			mQuaternion = mEndQuat;
			mRotateDir = rBackward;
		}
	}
	else if(mRotateDir == rBackward)
	{
		if(mCurrentRotateTimer <= mRotateTime)
		{
			float thirdPara = mCurrentRotateTimer/mRotateTime;
			mQuaternion = Quaternion::Slerp(mEndQuat, mStartQuat, thirdPara);
		}
		else
		{
			mCurrentRotateTimer = 0.0f;
			mQuaternion = mStartQuat;
			mRotateDir = rForward;
		}
	}
}


void Actor::OnUpdate(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState)
{
	if(this->mState == ActorState::Active)
	{
		for(auto c: this->mComponents)
		{
			if(c)
			{
				c->ProcessInput(keyState);
			}
		}
		OnProcessInput(keyState);
	}
}


void Actor::HandleKeyPress(int key)
{
	if(this->mState == ActorState::Active)
	{
		for(auto c: this->mComponents)
			c->HandleKeyPress(key);
	}
}


void Actor::OnProcessInput(const Uint8* keyState)
{
}

void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), 
		[](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
	});
}


Vector3 Actor::GetForward()
{
	Vector3 direction(cos(mRotation), sin(mRotation), 0.0f);
	direction.Normalize();
	return direction;
}


Vector3 Actor::GetRight()
{
	Vector3 direction(cos(mRotation+Math::PiOver2), 
		sin(mRotation+Math::PiOver2), 0.0f);
	direction.Normalize();
	return direction;
}


Vector3 Actor::GetQuatForward()
{
	Vector3 Rotation = Vector3::Transform(Vector3::UnitX, mQuaternion);
	Rotation.Normalize();
	return Rotation;
}


void Actor::CalcWorldTransform()
{
	Matrix4 scaleMatrix = Matrix4::CreateScale(mScale);
	Matrix4 positionMatrix =  Matrix4::CreateTranslation(mPosition);
	Matrix4 rotationMatrix = Matrix4::CreateRotationZ(mRotation);
	Matrix4 mQuaternionMatrix = Matrix4::CreateFromQuaternion(mQuaternion);

	this->mWorldTransform = scaleMatrix * rotationMatrix * 
							mQuaternionMatrix * positionMatrix;

	if(mParent != NULL)
	{
		if(mInheritScale)
		{
			this->mWorldTransform *= mParent->GetWorldTransform();
		}
		else
		{
			this->mWorldTransform *= mParent->CalcWorldRotTrans();
		}
	}
}
	

Matrix4 Actor::CalcWorldRotTrans()
{
	Matrix4 positionMatrix =  Matrix4::CreateTranslation(mPosition);
	Matrix4 rotationMatrix = Matrix4::CreateRotationZ(mRotation);
	Matrix4 mQuaternionMatrix = Matrix4::CreateFromQuaternion(mQuaternion);

	Matrix4 temp = rotationMatrix * mQuaternionMatrix * positionMatrix;

	if(mParent != NULL)
	{
		if(mInheritScale)
		{
			temp *= mParent->GetWorldTransform();
		}
		else
		{
			temp *= mParent->CalcWorldRotTrans();
		}
	}

	return temp;
}


Vector3 Actor::GetWorldPosition()
{
	return mWorldTransform.GetTranslation();
}
	

Vector3 Actor::GetWorldForward()
{
	return mWorldTransform.GetXAxis();
}


void Actor::AddChild(Actor* child)
{
	mChildren.push_back(child);
}
	

void Actor::RemoveChild(Actor* child)
{
	auto iter = std::find(mChildren.begin(), mChildren.end(), child);
	if (iter != mChildren.end())
	{
		auto iter2 = mChildren.end() - 1;
		std::iter_swap(iter, iter2);
		mChildren.pop_back();
	}
}










