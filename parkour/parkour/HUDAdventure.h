#pragma once
#include "UIComponent.h"
#include "Math.h"
#include <string>

class HUDAdventure: public UIComponent
{
public:
	HUDAdventure(Actor* parent);

	~HUDAdventure();

	void Update(float deltaTime);

	void Draw(class Shader* shader);

private:
	class Font* mFont;

	class Texture* mTimerText;
	float mTimer = 3.0f;

	class Texture* mScoreText;

	class Texture* mNewRecordText;

	class Texture* mLoseText;

	class Texture* mHeartText;
	class Texture* mHeartImgae;

	class Texture* mArrowTexture;
	class Texture* mBackgroundTexture;
	class Texture* mBlipTexture;

	Vector2 mRecordPos;
	Vector2 mTimerPos;
	Vector2 mScorePos;
	Vector2 mHeartTextPos;
	Vector2 mHeartImagePos;
	Vector2 mLosePos;

	bool breakRecord = false;
	bool showNewRecord = false;

	float showRecordTimer = 2.0f;
};


	//class Texture* mCoinText;
	//class Texture* mCoinImage;
	//Vector2 mCoinTextPos;
	//Vector2 mCoinImagePos;


