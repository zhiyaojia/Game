#include "HUDBuild.h"
#include "Texture.h"
#include "Font.h"
#include <string>
#include <iostream>
#include "Game.h"
#include "Actor.h"
#include "Renderer.h"
#include "Player.h"
#include "Block.h"
#include "PlayerMoveBuild.h"
#include "SegmentCast.h"
#include "MeshComponent.h"

using namespace std;


HUDBuild::HUDBuild(Actor* parent): UIComponent(parent)
{
	mOffTargetTex = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/image/TargetOff.png");
	mOnTargetTex = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/image/TargetOn.png");
}


HUDBuild::~HUDBuild()
{
	
}


void HUDBuild::Update(float deltaTime)
{
	if(!mOwner->GetGame()->player->GetComponent<PlayerMoveBuild>()->IsFlying()) 
	{
		if(mCurrentTarget)
		{
			if(mCurrentTarget->GetState() == ActorState::Active)
			{
				mCurrentTarget->GetComponent<MeshComponent>()->SetTextureIndex(mLastTargetTexIndex);
			}
		}
		return;
	}

	mOnTarget = false;

	mCastInfo.mPoint = Vector3::Zero;
	mCastInfo.mNormal = Vector3::Zero;
	mCastInfo.mActor = nullptr;
	
	const float cAimDist = 2000.0f;
	Vector3 start, dir;
	mOwner->GetGame()->GetRenderer()->GetScreenDirection(start, dir);
	LineSegment l(start, start + dir * cAimDist);
	
	if(SegmentCast(mOwner->GetGame()->Object, l, mCastInfo))
	{
		mOnTarget = true;
	}

	if(mCastInfo.mActor && mCastInfo.mActor->GetState() == ActorState::Active)
	{
		if(mCurrentTarget != mCastInfo.mActor)
		{
			if(mCurrentTarget)
			{
				mCurrentTarget->GetComponent<MeshComponent>()->SetTextureIndex(mLastTargetTexIndex);
			}
			mCurrentTarget = mCastInfo.mActor;
			mLastTargetTexIndex = mCastInfo.mActor->GetComponent<MeshComponent>()->GetTextureIndex();
			mCastInfo.mActor->GetComponent<MeshComponent>()->SetTextureIndex(mOnTargetTexIndex);
		}
	}
	else
	{
		if(mCurrentTarget)
		{
			if(mCurrentTarget->GetState() == ActorState::Active)
			{
				mCurrentTarget->GetComponent<MeshComponent>()->SetTextureIndex(mLastTargetTexIndex);
			}
		}
		mCurrentTarget = nullptr;
	}
}


void HUDBuild::Draw(class Shader* shader)
{
	if(!mOwner->GetGame()->player->GetComponent<PlayerMoveBuild>()->IsFlying()) {return;}
	Texture* cross = mOnTarget ? mOnTargetTex : mOffTargetTex;
	DrawTexture(shader, cross, Vector2::Zero, 2.0f);
}















