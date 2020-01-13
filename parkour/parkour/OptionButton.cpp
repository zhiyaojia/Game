#include "OptionButton.h"
#include "Texture.h"
#include "Font.h"
#include "Renderer.h"

using namespace std;

Option::Option(const std::string& name, Font* font, std::function<void()> function, bool global)
{
	mNameTex = font->RenderText(name, Color::White, 34, global);
	mFunction = function;
}


Option::~Option()
{
	if (mNameTex)
	{
		mNameTex->Unload();
		delete mNameTex;
	}
}


void Option::OnOption()
{
	if(mFunction)
	{
		mFunction();
	}
}


OptionButton::OptionButton
(
	class Game* game,
	const std::string& name, 
	const std::string& background,
	class Font* font, 
	const Vector2& pos, 
	const Vector2& dims, 
	std::vector<Option*> option
):Button(game, name, background, font, pos, dims)
{
	mOptions = option;
	mOptionIndex = 0;
}


OptionButton::~OptionButton()
{
	while (!mOptions.empty())
	{
		delete mOptions.back();
	}
}


void OptionButton::MoveRight()
{
	mOptionIndex++;
	if(mOptionIndex >= mOptions.size())
	{
		mOptionIndex = 0;
	}

    mOptions[mOptionIndex]->OnOption();
}


void OptionButton::MoveLeft()
{
	mOptionIndex--;
	if(mOptionIndex < 0)
	{
        mOptionIndex = static_cast<int>(mOptions.size())-1;
	}

    mOptions[mOptionIndex]->OnOption();
}





























