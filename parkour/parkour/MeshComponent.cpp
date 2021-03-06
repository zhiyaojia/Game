#include "MeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include <iostream>

MeshComponent::MeshComponent(Actor* owner, bool usesAlpha)
	:Component(owner)
	,mMesh(nullptr)
	,mTextureIndex(0)
	,mUsesAlpha(usesAlpha)
{
	mOwner->GetGame()->GetRenderer()->AddMeshComp(this, mUsesAlpha);
}

MeshComponent::~MeshComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveMeshComp(this, mUsesAlpha);
}

void MeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		// Set the world transform
		shader->SetMatrixUniform("uWorldTransform", 
			mOwner->GetWorldTransform());
		// Set the active texture
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}
		// Set the mesh's vertex array as active
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		// Draw
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}


void MeshComponent::ChangeTextureIndex(int x) 
{
	mTextureIndex += x; 

	if(mTextureIndex < 0)
	{
		mTextureIndex = mMesh->GetTextureNum()-1;
	}
	else if(mTextureIndex >= mMesh->GetTextureNum())
	{
		mTextureIndex = 0;
	}

	
}
















