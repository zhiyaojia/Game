#include "SettingScreen.h"
#include "Game.h"
#include <SDL/SDL.h>
#include "Renderer.h"
#include "OptionButton.h"
#include "MenuScreen.h"
#include "Texture.h"
#include <iostream>

using namespace std;

SettingScreen::SettingScreen(Game* game, bool language):UIScreen(game)
{
	mNextButtonPos = Vector2(-130, 145);
	mBackground = mGame->GetRenderer()->GetTexture("Assets/image/SmallMenu.png");
	mBGPos = Vector2(0.0f, 0.0f);

	mButtonCover = mGame->GetRenderer()->GetTexture("Assets/image/ButtonCoverSetting.png");

	if(language)
	{
		vector<Option*> mLanguageOptions;
		mLanguageOptions.push_back(new Option("EnglishOption", mFont, 
			[this](){ mGame->LoadText("Assets/Language/English.gptext");}));
		mLanguageOptions.push_back(new Option("ChineseOption", mFont, 
			[this](){ mGame->LoadText("Assets/Language/Chinese.gptext");}));
		AddOptionButton("LanguageButton", "EMPTY", mLanguageOptions);
	}

	vector<Option*> mSoundOptions;
	mSoundOptions.push_back(new Option("OnOption", mFont, [this](){ mGame->OpenAllSound();}));
	mSoundOptions.push_back(new Option("OffOption", mFont, [this](){ mGame->CloseAllSound();}));
	AddOptionButton("SoundButton", "EMPTY", mSoundOptions);

	vector<Option*> mMusicOptions;
	mMusicOptions.push_back(new Option("OnOption", mFont, [this](){ mGame->OpenAllMusic();}));
	mMusicOptions.push_back(new Option("OffOption", mFont, [this](){ mGame->CloseAllMusic();}));
	AddOptionButton("MusicButton", "EMPTY", mMusicOptions);

	vector<Option*> mVolumeOptions;
	mVolumeOptions.push_back(new Option("0", mFont, [this](){ mGame->SetVolume(0);}, true));
	mVolumeOptions.push_back(new Option("1", mFont, [this](){ mGame->SetVolume(13);}, true));
	mVolumeOptions.push_back(new Option("2", mFont, [this](){ mGame->SetVolume(26);}, true));
	mVolumeOptions.push_back(new Option("3", mFont, [this](){ mGame->SetVolume(40);}, true));
	mVolumeOptions.push_back(new Option("4", mFont, [this](){ mGame->SetVolume(53);}, true));
	mVolumeOptions.push_back(new Option("5", mFont, [this](){ mGame->SetVolume(66);}, true));
	mVolumeOptions.push_back(new Option("6", mFont, [this](){ mGame->SetVolume(80);}, true));
	mVolumeOptions.push_back(new Option("7", mFont, [this](){ mGame->SetVolume(96);}, true));
	mVolumeOptions.push_back(new Option("8", mFont, [this](){ mGame->SetVolume(115);}, true));
	mVolumeOptions.push_back(new Option("9", mFont, [this](){ mGame->SetVolume(128);}, true));
	AddVolumeButton("VolumeButton", "EMPTY", mVolumeOptions);

	AddClickButton("ConfirmButton", "EMPTY", [this, language](){ 
		if(language){mGame->CloseSettingMenu();}
		else{Close();}
	}, Vector2(0, -150.0f));
}


void SettingScreen::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);
	if (key == SDLK_ESCAPE)
	{
		mGame->CloseSettingMenu();
	}
}


void SettingScreen::AddVolumeButton(const string& name, const string& background, vector<class Option*> option)
{
	Vector2 dims(1440, 50);
	OptionButton* b = new OptionButton(mGame, name, background, mFont, mNextButtonPos, dims, option);

	mGame->SetVolumeButton(b);
	
	mButtons.emplace_back(b);
	mOptionButtons.emplace_back(b);
	
	mNextButtonPos.y -= 50.0f + 20.0f;
}

















