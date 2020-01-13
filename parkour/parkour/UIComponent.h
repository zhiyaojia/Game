#pragma once
#include "Math.h"
#include "Component.h"
#include "SegmentCast.h"

class UIComponent : public Component
{
public:
	UIComponent(class Actor* owner);
	~UIComponent();
	
	virtual void Draw(class Shader* shader);

	// override for HUDAdventure
	virtual void addCoin() {}
	virtual void updateCheckpointText(std::string text) {}

	// override for HUDBuild
    virtual CastInfo GetCastInfo() {return mCastInfo;}

protected:
	// Helper to draw a texture
	void DrawTexture(class Shader* shader, class Texture* texture,
					 const Vector2& offset = Vector2::Zero,
					 float scale = 1.0f, float angle = 0.0f);

	struct CastInfo mCastInfo;
};
