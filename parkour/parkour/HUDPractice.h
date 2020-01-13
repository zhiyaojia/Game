#pragma once
#include "UIComponent.h"
#include "Math.h"
#include <string>

class HUDPractice: public UIComponent
{
public:
	HUDPractice(Actor* parent);

	~HUDPractice();

	void Update(float deltaTime);

	void Draw(class Shader* shader);

	void updateCheckpointText(std::string text);

private:
	class Font* mFont;

    class Texture* mCheckpointTexture;
  	
  	float mCheckpointTimer = 5.0f;
};
