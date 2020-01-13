#pragma once
#include "Button.h"
#include <vector>

class Option
{
public:
	Option(const std::string& name, class Font* font, std::function<void()> function, bool global = false);
	~Option();

	class Texture* GetNameTex() { return mNameTex;}

	void OnOption();

private:
	class Texture* mNameTex;
	
	std::function<void()> mFunction;
};


class OptionButton: public Button
{
public:
	OptionButton(class Game* game, const std::string& name, const std::string& background,class Font* font, 
		const Vector2& pos, const Vector2& dims, std::vector<Option*> option);
	~OptionButton();

	void MoveRight();
	void MoveLeft();

	class Texture* GetOptionTex() {return mOptions[mOptionIndex]->GetNameTex();}

private:
	int mOptionIndex;
	
	std::vector<Option*> mOptions;
};



















