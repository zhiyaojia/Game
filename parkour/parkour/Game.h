#pragma once
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include <queue>

class Game
{
public:
	enum GameState
	{
		Gameplay,
		GamePaused,
		Quit,
		Menu,
		GameToMenu,
		MenuToGame
	};

	enum GameMode
	{
		Adventure,
		Build,
		Maze,
		Tutorial,
		Practice
	};

	enum Language
	{
		English,
		Chinese
	};

	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void addToCameraArray(class SecurityCamera* e);
	void removeFromCameraArray(class SecurityCamera* e);

	void addCoin(){coinNum++;}
	int getCoin(){return coinNum;}

	void SetAdventureScore(int score) {mAdvantureScore = score;}
	int GetAdventureScore() {return mAdvantureScore;}

	void SetFinalPos(Vector3 pos) {mFinalBlockPos = pos;}
	Vector3 GetFinalPos() {return mFinalBlockPos;}

	void SetGameMode(GameMode g) {mCurrentMode = g;}
	GameMode GetGameMode() {return mCurrentMode;} 

	GameState GetState() const { return mGameState; }
	void ChangeState(GameState state) { mGameState = state; }

	Language GetLanguage() {return mCurrentLanguage;}
	void SetLanguage(Language l) {mCurrentLanguage = l;}

	void StartGameAgain() {UnloadData(); LoadData();}

	void SetMazeMap(std::string m);

// Sound Functions
	Mix_Chunk* GetSound(const std::string& fileName);
	void LoudSound();
	void UnLoudSound();

	void ResumeGameSound();
	void PauseGameSound();

	void OpenAllSound();
	void CloseAllSound();

	void OpenAllMusic();
	void CloseAllMusic();

	bool GetSoundOn() {return mSoundOn;}

	void SetVolume(int volume);

	bool ShowVolumeButton() {return (mSoundOn || mMusicOn);}
	class OptionButton* GetVolumeButton() {return mVolumeButton;}
	void SetVolumeButton(class OptionButton* b) {mVolumeButton = b;}

	void SetBackGroundMusic(std::string music) {BackGroundMuiscName = music;}

	int mGameMusic, mRunningSound, mJumpSound, mSlideSound, mLandSound,
		mMenuSound, mLaserSound, mCoinSound, mCheckPointSound, mCameraCatchSound;


// UI Functions
	const std::vector<class UIScreen*>& GetUIStack() { return mUIStack; }
	void PushUI(class UIScreen* screen);
	bool IsFirstUI(class UIScreen* screen);
	void CloseSettingMenu();


// Font Functions
	class Font* GetFont(const std::string& fileName);
	void LoadText(const std::string& fileName);
	const std::string& GetText(const std::string& key);


// Public Variables
	class Player* player;

// Public Containers
	std::vector<class Actor*> Object;
	std::vector<class Actor*> mActors;
	std::vector<std::string> mMazeMap;
	std::vector<std::string> mMazeMusic;
	std::vector<class SecurityCamera*> cameraArray;
	std::vector<const std::string> mAdvantureMap;
	std::queue<class Checkpoint*> CheckpointArray;


// Helper Functions
	void ListFileName(const char *path, int level, std::vector<std::string> &strvec);

	class Renderer* GetRenderer() {	return mRenderer; }

	void LoadAdvanture();
	void LoadAdvantureMaps();

private:
	void ProcessInput();
	void HandleKeyPress(int key);
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
	void ReadDataFile();
	void WriteDataFile();

	std::unordered_map<std::string, Mix_Chunk*> mSounds;
	std::string BackGroundMuiscName = "";

	class Renderer* mRenderer;

	// All the actors in the game
	std::vector<class UIScreen*> mUIStack;
	std::unordered_map<std::string, class Font*> mFonts;

	// Map for text localization
	std::unordered_map<std::string, std::string> mText;

	Uint32 mTicksCount;
	bool mIsRunning;

    int coinNum = 0;

    int mSoundNum;
	int mGameVolume;

	class OptionButton* mVolumeButton = NULL;
	bool mSoundOn, mMusicOn;

	Vector3 mFinalBlockPos = Vector3(-500.0f, 0.0f, 0.0f);

	bool randomMap = false;
	int adventureMapIndex = 0;
	int mAdvantureScore;

	std::string mMazeMapName = "Assets/Map/Maze/Maze1.json";

	GameMode mCurrentMode;
	GameState mGameState;
	Language mCurrentLanguage;
};















