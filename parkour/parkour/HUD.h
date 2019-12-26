#pragma once
#include "UIComponent.h"
#include "Math.h"
#include <string>

class HUD: public UIComponent
{
public:
	HUD(Actor* parent);

	~HUD();

	void Update(float deltaTime);

	void Draw(class Shader* shader);

	void updateCheckpointText(std::string text);

	void addCoin();

private:
	class Font* mFont;

    class Texture* mTimerText;
    class Texture* mCoinText;
    class Texture* mCheckpointTexture;
    class Texture* mBackgroundTexture;
    class Texture* mArrowTexture;
    class Texture* mBlipTexture;

    Vector2 coinPos;
};
