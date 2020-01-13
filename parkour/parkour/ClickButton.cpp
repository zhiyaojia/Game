#include "ClickButton.h"

using namespace std;

ClickButton::ClickButton
(
	class Game* game, 
	const std::string& name, 
	const std::string& background,
	class Font* font, 
	std::function<void()> onClick, 
	const Vector2& pos, 
	const Vector2& dims
):Button(game, name, background, font, pos, dims)
{
	mOnClick = onClick;
}


void ClickButton::OnClick()
{
	if (mOnClick)
	{
		mOnClick();
	}
}




