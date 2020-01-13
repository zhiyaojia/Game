#include "Button.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Font.h"
#include "Game.h"
#include <iostream>

using namespace std;

Button::Button(class Game* game, const std::string& name, const std::string& background, Font* font,
	const Vector2& pos, const Vector2& dims)
	:mGame(game)
	,mNameTex(nullptr)
	,mFont(font)
	,mPosition(pos)
	,mDimensions(dims)
	,mHighlighted(false)
    ,mLastHighlight(false)
{
	mBackground = (background == "EMPTY")?nullptr:mGame->GetRenderer()->GetTexture(background);
	
	mName = name;
	mButtonOn = mFont->RenderText(mName, Color::Red, 40);
	mButtonOff = mFont->RenderText(mName, Color::DarkRed, 40);
}

Button::~Button()
{
	if (mNameTex)
	{
		mNameTex->Unload();
		delete mNameTex;
	}
}

void Button::SetName(const std::string& name)
{
	mName = name;

	if (mNameTex)
	{
		mNameTex->Unload();
		delete mNameTex;
		mNameTex = nullptr;
	}
	mNameTex = mFont->RenderText(mName);
}

bool Button::ContainsPoint(const Vector2& pt) const
{
	bool no = pt.x < (mPosition.x - mDimensions.x / 2.0f) ||
		pt.x > (mPosition.x + mDimensions.x / 2.0f) ||
		pt.y < (mPosition.y - mDimensions.y / 2.0f) ||
		pt.y > (mPosition.y + mDimensions.y / 2.0f);
	return !no;
}


void Button::SetHighlighted(bool sel) 
{ 
	mLastHighlight = mHighlighted;
	mHighlighted = sel; 
}
















