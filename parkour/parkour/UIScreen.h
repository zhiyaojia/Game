#pragma once
#include "Math.h"
#include <cstdint>
#include <string>
#include <functional>
#include <vector>

class UIScreen
{
public:
	UIScreen(class Game* game);
	virtual ~UIScreen();
	// UIScreen subclasses can override these
	virtual void Update(float deltaTime);
	virtual void Draw(class Shader* shader);
	virtual void ProcessInput(const uint8_t* keys);
	virtual void HandleKeyPress(int key);
	// Tracks if the UI is active or closing
	enum UIState
	{
		EActive,
		EClosing
	};

	Vector2 GetMousePosition();

	void Close() { mState = EClosing;}
	// Get state of UI screen
	UIState GetState() const { return mState;}

	// Change the title text
	void SetTitle(const std::string& text,
				  const Vector3& color = Color::White,
				  int pointSize = 40);

	void AddClickButton(const std::string& name, const std::string& background, 
						std::function<void()> onClick, Vector2 pos = Vector2(999, 999));
	void AddOptionButton(const std::string& name, const std::string& background, std::vector<class Option*> option);
protected:
	// Helper to draw a texture
	void DrawTexture(class Shader* shader, class Texture* texture,
					 const Vector2& offset = Vector2::Zero,
					 float scale = 1.0f);
	// Sets the mouse mode to relative or not
	void SetRelativeMouseMode(bool relative);
	class Game* mGame;
	
	float mArrowPos = 50.0f;

	class Font* mFont;
	class Texture* mTitle;
	class Texture* mBackground;
	class Texture* mButtonCover;
	class Texture* ArrowLeft;
    class Texture* ArrowRight;

	// Configure positions
	Vector2 mTitlePos;
	Vector2 mNextButtonPos;
	Vector2 mBGPos;
	float mButtonCoverPos;

	// State
	UIState mState;
	// List of buttons
	std::vector<class Button*> mButtons;
	std::vector<class ClickButton*> mClickButtons;
	std::vector<class OptionButton*> mOptionButtons;
    std::vector<std::pair<class Texture*, Vector2 >> mTextTextures;
};
























