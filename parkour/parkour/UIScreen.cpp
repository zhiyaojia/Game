#include "UIScreen.h"
#include "Button.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Font.h"
#include "ClickButton.h"
#include "OptionButton.h"
#include <iostream>

using namespace std;

UIScreen::UIScreen(Game* game)
	:mGame(game)
	,mTitle(nullptr)
	,mBackground(nullptr)
	,mTitlePos(0.0f, 100.0f)
	,mNextButtonPos(0.0f, 0.0f)
	,mBGPos(0.0f, 0.0f)
	,mState(EActive)
	,mButtonCoverPos(0.0f)
{
	mGame->PushUI(this);
	mFont = mGame->GetFont("Assets/Font/HuaWenXiHei-1.ttf");
	mButtonCover = mGame->GetRenderer()->GetTexture("Assets/image/ButtonCover.png");
	ArrowLeft = game->GetRenderer()->GetTexture("Assets/image/ArrowLeft.png");
	ArrowRight = game->GetRenderer()->GetTexture("Assets/image/ArrowRight.png");
}

UIScreen::~UIScreen()
{
	if (mTitle)
	{
		mTitle->Unload();
		delete mTitle;
	}

	for (auto b : mButtons)
	{
		delete b;
	}
	mButtons.clear();
}

void UIScreen::Update(float deltaTime)
{
	
}

void UIScreen::Draw(Shader* shader)
{
	// Draw background (if exists)
	if (mBackground)
	{
		DrawTexture(shader, mBackground, mBGPos);
	}
	// Draw title (if exists)
	if (mTitle)
	{
		DrawTexture(shader, mTitle, mTitlePos);
	}
	// Draw buttons
	for (auto b : mButtons)
	{
		if(b == mGame->GetVolumeButton() && !mGame->ShowVolumeButton()){continue;}
		float height = b->GetPosition().y * 0.9f;
		if(b->GetHighlighted() && mGame->IsFirstUI(this))
		{
			DrawTexture(shader, mButtonCover, Vector2(mButtonCoverPos, height));
			DrawTexture(shader, b->GetButtonOnTex(),Vector2(b->GetPosition().x, height));
		}
		else
		{
			Button* t = static_cast<OptionButton*>(b);
			if (t != nullptr)
			{
				DrawTexture(shader, b->GetButtonOffTex(), Vector2(0, height));
			}
			else
			{
				DrawTexture(shader, b->GetButtonOffTex(), Vector2(b->GetPosition().x, height));
			}
		}
	}

	for(auto b: mOptionButtons)
	{
		if(b == mGame->GetVolumeButton() && !mGame->ShowVolumeButton()){continue;}

		if(b->GetHighlighted())
		{
			if(b == mGame->GetVolumeButton() && !mGame->ShowVolumeButton()){continue;}
			float height = b->GetPosition().y * 0.9f;
			DrawTexture(shader, ArrowLeft, Vector2(mArrowPos, height));
			DrawTexture(shader, b->GetOptionTex(), Vector2(mArrowPos + 100.0f, height));
			DrawTexture(shader, ArrowRight, Vector2(mArrowPos + 200.0f, height));
		}
	}

	for(auto p: mTextTextures)
	{
        DrawTexture(shader, p.first, p.second);
	}
}


Vector2 UIScreen::GetMousePosition()
{
	// Get position of mouse
	int x, y;
	SDL_GetMouseState(&x, &y);
	// Convert to (0,0) center coordinates
	Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
	mousePos.x -= mGame->GetRenderer()->GetScreenWidth() * 0.5f;
	mousePos.y = mGame->GetRenderer()->GetScreenHeight() * 0.5f - mousePos.y;

	return mousePos;
}


void UIScreen::ProcessInput(const uint8_t* keys)
{
	// Do we have buttons?
	if (!mButtons.empty())
	{   
		Vector2 mousePos = GetMousePosition();

		// Highlight any buttons
		for (auto b : mButtons)
		{
			if(b == mGame->GetVolumeButton() && !mGame->ShowVolumeButton()){continue;}

			if (b->ContainsPoint(mousePos))
			{
				Texture* buttonBackground = b->GetBackgroundTex();
				if(buttonBackground)
				{
					mBackground = buttonBackground;
				}
				b->SetHighlighted(true);
			}
			else
			{
				b->SetHighlighted(false);
			}

			if(b->GetFirstHighlighted())
			{
				Mix_PlayChannel(mGame->mMenuSound, mGame->GetSound("Assets/Sounds/highlight.wav"), 0);
			}
		}
	}
}

void UIScreen::HandleKeyPress(int key)
{
	Vector2 mousePos = GetMousePosition();
	switch (key)
	{
		case SDL_BUTTON_LEFT:
			if (!mButtons.empty())
			{
				for (auto b : mClickButtons)
				{
					if (b->GetHighlighted())
					{
						b->OnClick();
						break;
					}
				}
			}
			break;

		case SDLK_a:
			if (!mButtons.empty())
			{
				for (auto b : mOptionButtons)
				{
					if (b->GetHighlighted())
					{
						b->MoveLeft();
						break;
					}
				}
			}
			break;

		case SDLK_d:
			if (!mButtons.empty())
			{
				for (auto b : mOptionButtons)
				{
					if (b->GetHighlighted())
					{
						b->MoveRight();
						break;
					}
				}
			}
			break;

		case SDLK_p:
			cout<<"Mouse pos: "<<mousePos.x<<", "<<mousePos.y<<endl;
			break;

		default:
			break;
	}
}


void UIScreen::SetTitle(const std::string& text,const Vector3& color,int pointSize)
{
	if (mTitle)
	{
		mTitle->Unload();
		delete mTitle;
		mTitle = nullptr;
	}
	mTitle = mFont->RenderText(text, color, pointSize);
}


void UIScreen::AddClickButton(const string& name, const string& background, 
								function<void()> onClick, Vector2 pos)
{
	bool next = (pos.x == 999 && pos.y == 999);
	Vector2 position = next?mNextButtonPos:pos;
	Vector2 dims(1440, 50);
	ClickButton* b = new ClickButton(mGame, name, background, mFont, onClick, position, dims);
	
	mButtons.emplace_back(b);
	mClickButtons.emplace_back(b);
	
	if(next) {mNextButtonPos.y -= 50.0f + 20.0f;}
}


void UIScreen::AddOptionButton(const string& name, const string& background, vector<Option*> option)
{
	Vector2 dims(1440, 50);
	OptionButton* b = new OptionButton(mGame, name, background, mFont, mNextButtonPos, dims, option);
	
	mButtons.emplace_back(b);
	mOptionButtons.emplace_back(b);
	
	mNextButtonPos.y -= 50.0f + 20.0f;
}


void UIScreen::DrawTexture(class Shader* shader, class Texture* texture,
				 const Vector2& offset, float scale)
{
	// Scale the quad by the width/height of texture
	Matrix4 scaleMat = Matrix4::CreateScale(
		static_cast<float>(texture->GetWidth()) * scale,
		static_cast<float>(texture->GetHeight()) * scale,
		1.0f);
	// Translate to position on screen
	Matrix4 transMat = Matrix4::CreateTranslation(
		Vector3(offset.x, offset.y, 0.0f));	
	// Set world transform
	Matrix4 world = scaleMat * transMat;
	shader->SetMatrixUniform("uWorldTransform", world);
	// Set current texture
	texture->SetActive();
	// Draw quad
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void UIScreen::SetRelativeMouseMode(bool relative)
{
	if (relative)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		// Make an initial call to get relative to clear out
		SDL_GetRelativeMouseState(nullptr, nullptr);
	}
	else
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}

