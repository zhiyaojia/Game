#include "LaserComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Block.h"
#include "Player.h"
#include "Random.h"
#include "PlayerMoveAdventure.h"
#include "PlayerMove.h"
#include <iostream>

using namespace std;

LaserComponent::LaserComponent(Actor* owner, bool random, float length):MeshComponent(owner, true)
{	
	SetMesh(mOwner->GetGame()->GetRenderer()->GetMesh("Assets/GameObject/Laser.gpmesh"));
	mLength = length;
	mRandom = random;
}
	
void LaserComponent::Update(float deltaTime)
{
	LaserArray.clear();
	
	if(mRandom)
	{
		if(onStatus)
		{
			timer += deltaTime;
			if(timer > mStatusTime)
			{
				onStatus = false;
				timer = 0.0f;
			}
		}
		else
		{
			int x = Random::GetIntRange(0, 2);
			mActive = (x==0);
			onStatus = true;
		}
	}
	if(!mActive){return;}

	LineSegment FirstLaser;
    FirstLaser.mStart = mOwner->GetWorldPosition();
	FirstLaser.mEnd = FirstLaser.mStart + (mLength * mOwner->GetWorldForward());

	CastInfo info;

	if(SegmentCast((Actor*)mOwner->GetGame()->player, FirstLaser, info))
	{
		FirstLaser.mEnd = info.mPoint;
		
        Game::GameMode g = mOwner->GetGame()->GetGameMode();
		if(g == Game::Adventure)
		{
			mOwner->GetGame()->player->GetComponent<PlayerMoveAdventure>()->GetLaserHit();
		}
		else if(g == Game::Tutorial || g == Game::Practice)
		{
			mOwner->GetGame()->player->GetComponent<PlayerMove>()->GetLaserHit();
		}

		Mix_PlayChannel(mOwner->GetGame()->mLaserSound, mOwner->GetGame()->GetSound("Assets/Sounds/Laser.wav"), 0);
	}
	else
	{
		if(SegmentCast(mOwner->GetGame()->Object, FirstLaser, 
					info, mOwner->GetParent()))
		{
			FirstLaser.mEnd = info.mPoint;
		}
	}
	LaserArray.push_back(FirstLaser);
	
	Vector3 LaserForward = mOwner->GetQuatForward();

	bool NoMoreMirror = true;

	if(info.mActor != NULL && info.mActor->GetMirror())
	{
		NoMoreMirror = false;
	}
	
	while(!NoMoreMirror)
	{
		LineSegment NewLaser;
		NewLaser.mStart = info.mPoint;

		LaserForward = Vector3::Reflect(LaserForward, info.mNormal);
		NewLaser.mEnd = NewLaser.mStart + (mLength * LaserForward);

		if(SegmentCast((Actor*)mOwner->GetGame()->player, NewLaser, info))
		{
			NewLaser.mEnd = info.mPoint;
		}
		else
		{
			if(SegmentCast(mOwner->GetGame()->Object, NewLaser, info, info.mActor))
			{
                NewLaser.mEnd = info.mPoint;
			}
		}
		LaserArray.push_back(NewLaser);

		if(info.mActor != NULL && info.mActor->GetMirror())
		{
			NoMoreMirror = false;
		}
		else
		{
			NoMoreMirror = true;
		}
	}
}

void LaserComponent::Draw(Shader* shader)
{
	if(!mActive){return;}

	if (mMesh)
	{
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}
		
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
	
		for(auto Laser: LaserArray)
		{
			shader->SetMatrixUniform("uWorldTransform", Transform(Laser));
			glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
		}
	}
}


Matrix4 LaserComponent::Transform(LineSegment Laser)
{
	Matrix4 scaleMatrix = Matrix4::CreateScale(Laser.Length(), 1, 1);

 	Vector3 Middle = Laser.mStart + Laser.mEnd;
 	Middle.x /= 2;
 	Middle.y /= 2;
 	Middle.z /= 2;

	Matrix4 positionMatrix =  Matrix4::CreateTranslation(Middle);

	Quaternion RotationQuaternion;
	
	Vector3 forward = Laser.mEnd - Laser.mStart;
	forward.Normalize();
    
	float dot = Vector3::Dot(Vector3::UnitX, forward);
	
	if(Math::NearZero(1 - dot))
	{
		RotationQuaternion = Quaternion::Identity;
	}
	else if(Math::NearZero(1 + dot))
	{
		RotationQuaternion = Quaternion(Vector3::UnitZ, Math::Pi);
	}
	else
	{
		float angle = Math::Acos(dot);
		Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
		axis.Normalize();

		RotationQuaternion = Quaternion(axis, angle);
	}

	Matrix4 rotationMatrix = Matrix4::CreateFromQuaternion(RotationQuaternion);
	
	Matrix4 mWorldTransform = scaleMatrix * rotationMatrix * positionMatrix;

	return mWorldTransform;
}


    











