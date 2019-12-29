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
#include "PlayerMove.h"
#include <iostream>

using namespace std;

LaserComponent::LaserComponent(Actor* owner):MeshComponent(owner, true)
{	
	SetMesh(mOwner->GetGame()->GetRenderer()->GetMesh("Assets/Laser.gpmesh"));
}
	
void LaserComponent::Update(float deltaTime)
{
	LaserArray.clear();

	LineSegment FirstLaser;
    FirstLaser.mStart = mOwner->GetWorldPosition();
	FirstLaser.mEnd = FirstLaser.mStart + (500.0f * mOwner->GetWorldForward());

	CastInfo info;

	if(SegmentCast((Actor*)mOwner->GetGame()->player, FirstLaser, info))
	{
		FirstLaser.mEnd = info.mPoint;
		mOwner->GetGame()->player->GetComponent<PlayerMove>()->Respawn();
		Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Laser.wav"), 0);
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
		NewLaser.mEnd = NewLaser.mStart + (500.0f * LaserForward);

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


    











