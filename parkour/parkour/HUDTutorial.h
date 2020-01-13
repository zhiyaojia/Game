#pragma once
#include "UIComponent.h"
#include "Math.h"
#include <string>

class HUDTutorial: public UIComponent
{
public:
	HUDTutorial(Actor* parent);

	~HUDTutorial();

	void Update(float deltaTime);

	void Draw(class Shader* shader);

	void updateCheckpointText(std::string text);

private:
	class Font* mFont;

	class Texture* mHeartText;
	class Texture* mHeartImgae;

	class Texture* mArrowTexture;
	class Texture* mBackgroundTexture;
	class Texture* mBlipTexture;

	class Texture* mCheckpointTexture;
	
	float mCheckpointTimer = 5.0f;

	Vector2 mHeartTextPos;
	Vector2 mHeartImagePos;
};





