#pragma once
#include "Button.h"

class ClickButton: public Button
{
public:
	ClickButton(class Game* game, const std::string& name, const std::string& background,
		class Font* font, std::function<void()> onClick, 
		const Vector2& pos, const Vector2& dims);

	void OnClick();

private:
	std::function<void()> mOnClick;
};
