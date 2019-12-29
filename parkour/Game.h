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
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	Mix_Chunk* GetSound(const std::string& fileName);

	class Renderer* GetRenderer() {	return mRenderer; }

	int background_music;

	class Player* player;

	std::vector<class Actor*> Object;

	std::queue<class Checkpoint*> CheckpointArray;

	std::vector<class SecurityCamera*> cameraArray;
	void addToCameraArray(class SecurityCamera* e);
	void removeFromCameraArray(class SecurityCamera* e);

	void SetLevel(std::string level){mNextLevel = level;}

	void LoadNextLevel();

	void SetBackGroundMusic(std::string music) {BackGroundMuiscName = music;}

	void updateTimer(float deltatime){timer += deltatime;}
	float getTimer() {return timer;}

	void addCoin(){coinNum++;}
	int getCoin(){return coinNum;}

	void updateCheckPointTextTimer(float deltatime) {checkpointTextTimer += deltatime;}
	float getCheckPointTextTimer() {return checkpointTextTimer;}
	void resetCheckPointTextTimer() {checkpointTextTimer = 0.0f;}

	void updateCheckPointText(std::string text) {checkpointText = text;}
	std::string getCheckPointText() {return checkpointText;}

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, Mix_Chunk*> mSounds;
	std::string BackGroundMuiscName = "";

	// All the actors in the game
	std::vector<class Actor*> mActors;

	class Renderer* mRenderer;

	Uint32 mTicksCount;
	bool mIsRunning;

	float timer = 0.0;
	float checkpointTextTimer = 0.0f;
	std::string checkpointText = "";

    int coinNum = 0;

	std::string mNextLevel;
};















