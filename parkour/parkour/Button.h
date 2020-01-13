#pragma once
#include "Math.h"
#include <cstdint>
#include <string>
#include <functional>

class Button
{
public:
	Button(class Game* game, const std::string& name, const std::string& background, class Font* font,
		const Vector2& pos, const Vector2& dims);
	~Button();

	// Set the name of the button
	void SetName(const std::string& name);
	
	class Texture* GetNameTex() { return mNameTex;}
	class Texture* GetBackgroundTex() { return mBackground;}
	class Texture* GetButtonOnTex() {return mButtonOn;}
	class Texture* GetButtonOffTex() {return mButtonOff;}

	const Vector2& GetPosition() const { return mPosition;}

	void SetHighlighted(bool sel);
	bool GetHighlighted() const { return mHighlighted;}
	bool GetFirstHighlighted() const { return (mHighlighted&&!mLastHighlight);}

	// Returns true if the point is within the button's bounds
	bool ContainsPoint(const Vector2& pt) const;

protected:	
	bool mHighlighted;
	bool mLastHighlight;

	std::string mName;

	Vector2 mPosition;
	Vector2 mDimensions;

	class Game* mGame;

	class Font* mFont;

	class Texture* mNameTex;
	class Texture* mBackground;
	class Texture* mButtonOn;
	class Texture* mButtonOff;
};












