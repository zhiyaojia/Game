#pragma once
#include "UIComponent.h"
#include "Math.h"
#include <string>

class HUDMaze: public UIComponent
{
public:
	HUDMaze(Actor* parent);

	~HUDMaze();

	void Update(float deltaTime);

	void Draw(class Shader* shader);

	void SetShowText(int s) {showText = s;}

private:
	class Font* mFont;

	class Texture* mTimerText;
	class Texture* mSuccessText;
	class Texture* mLoseText;

	Vector2 mPos;

	float mTimer;

	int showText = 0; // -1 for lose, 1 for success
};
