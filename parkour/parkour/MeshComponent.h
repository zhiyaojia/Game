#pragma once
#include "Component.h"
#include <iostream>
#include <cstddef>

class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner, bool usesAlpha = false);
	~MeshComponent();
	// Draw this mesh component
	virtual void Draw(class Shader* shader);
	// Set the mesh/texture index used by mesh component
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }

	void SetTextureIndex(int index) { mTextureIndex = index;}
	int GetTextureIndex() {return mTextureIndex;}

	void ChangeTextureIndex(int x);

protected:
	class Mesh* mMesh;
	int mTextureIndex;
	bool mUsesAlpha;
};
