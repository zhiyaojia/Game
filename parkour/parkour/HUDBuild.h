#pragma once
#include "UIComponent.h"
#include "Math.h"
#include <string>

class HUDBuild: public UIComponent
{
public:
	HUDBuild(Actor* parent);

	~HUDBuild();

	void Update(float deltaTime);

	void Draw(class Shader* shader);

    CastInfo GetCastInfo() {return mCastInfo;}

    void BreakBlock() {mCurrentTarget = nullptr;}

    int GetTargetIndex() {return mLastTargetTexIndex;}
    void SetTargetIndex(int index) {mLastTargetTexIndex = index;}

private:
	bool mOnTarget = false;

	struct CastInfo mCastInfo;

	class Texture* mOnTargetTex;
	class Texture* mOffTargetTex;

	class Actor* mCurrentTarget = nullptr;
	const int mOnTargetTexIndex = 13;
	int mLastTargetTexIndex = 0;
};
