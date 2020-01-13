#include "Game.h"
#include "Actor.h"
#include "Renderer.h"
#include "Random.h"
#include "Player.h"
#include "PlayerMoveBuild.h"
#include "MeshComponent.h"
#include "SecurityCamera.h"
#include "LevelLoader.h"
#include "Arrow.h"
#include "Checkpoint.h"
#include "Coin.h"
#include "SDL/SDL_ttf.h"
#include "Block.h"
#include "LaserMine.h"
#include "Font.h"
#include "LaserComponent.h"
#include "PauseMenu.h"
#include "MenuScreen.h"
#include "UIScreen.h"
#include "GameModeScreen.h"
#include "CSVHelper.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <rapidjson/document.h>

using namespace std;

Game::Game()
:mIsRunning(true)
{
	mGameState = Menu;
	mCurrentMode = Build;
}

bool Game::Initialize()
{
	Random::Init();

	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	this->mRenderer = new Renderer(this);
	if (!this->mRenderer->Initialize())
	{
		SDL_Log("Unable to initialize render");
		return false;
	}

	// View Setup
	this->mRenderer->SetProjectionMatrix(Matrix4::CreatePerspectiveFOV(1.22f, 1024.0f, 768.0f, 10.0f, 10000.0f));

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_AllocateChannels(32);

	TTF_Init();

	mTicksCount = SDL_GetTicks();

	LoadText("Assets/Language/English.gptext");

	LoudSound();

	ReadDataFile();
	LoadAdvantureMaps();

	ListFileName("Assets/Map/Maze", 0, mMazeMap);

	vector<string> temp;
	ListFileName("Assets/Sounds/Music", 0, temp);

	for(auto s: temp)
	{
		if(s != ".DS_Store") {mMazeMusic.push_back(s);}
	}

	new MenuScreen(this);
	
	return true;
}

void Game::RunLoop()
{
	while (mGameState != Quit)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				break;

			case SDL_KEYDOWN:
				if (!event.key.repeat)
				{
					if (mGameState == Gameplay)
					{
						HandleKeyPress(event.key.keysym.sym);
						player->HandleKeyPress(event.key.keysym.sym);
					}
					else if(mGameState == GamePaused || mGameState == Menu)
					{
						if (!mUIStack.empty())
						{
							mUIStack.back()->
								HandleKeyPress(event.key.keysym.sym);
						}
					}
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (mGameState == Gameplay)
				{
					player->HandleKeyPress(event.button.button);
				}
				else if(mGameState == GamePaused || mGameState == Menu)
				{
					if (!mUIStack.empty())
					{
						mUIStack.back()->
							HandleKeyPress(event.button.button);
					}
				}
				break;

			case SDL_MOUSEWHEEL:
			{
				if(mCurrentMode == Build)
				{
					player->GetComponent<PlayerMoveBuild>()->ChangeTexture(event.wheel.y);
				}
				break;
			}

			default:
				break;
		}
	}
	
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (mGameState == Gameplay)
	{
		for (auto actor : mActors)
		{
			if (actor->GetState() == ActorState::Active)
			{
				actor->ProcessInput(state);
			}
		}
	}
	else if(mGameState == GamePaused || mGameState == Menu)
	{
		if (!mUIStack.empty())
		{
			mUIStack.back()->ProcessInput(state);
		}
	}
}


void Game::HandleKeyPress(int key)
{
	switch (key)
	{
		case SDLK_q:
			mGameState = Quit;
			break;

		default:
			break;
	}
}


void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.01667f)
	{
		deltaTime = 0.01667f;
	}
	mTicksCount = SDL_GetTicks();

	for (auto ui : mUIStack)
	{
		if (ui->GetState() == UIScreen::EActive)
		{
			ui->Update(deltaTime);
		}
	}
	// Delete any UIScreens that are closed
	auto iter = mUIStack.begin();
	while (iter != mUIStack.end())
	{
		if ((*iter)->GetState() == UIScreen::EClosing)
		{
			delete *iter;
			iter = mUIStack.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	
	if (mGameState == Gameplay)
	{
		std::vector<Actor*> copy = mActors;
		for (auto actor : copy)
		{
			actor->Update(deltaTime);
		}

		std::vector<Actor*> destroyActors;
		for (auto actor : mActors)
		{
			if (actor->GetState() == ActorState::Destroy)
			{
				destroyActors.emplace_back(actor);
			}
		}

		for (auto actor : destroyActors)
		{
			delete actor;
		}
	}
	else if(mGameState == MenuToGame)
	{
		LoadData();
		SDL_SetRelativeMouseMode(SDL_TRUE); 
		SDL_GetRelativeMouseState(nullptr, nullptr);
		ChangeState(Gameplay);
	}
	else if(mGameState == GameToMenu)
	{
		UnloadData();
		SDL_SetRelativeMouseMode(SDL_FALSE);
		new GameModeScreen(this);
		ChangeState(Menu);
	}
}

void Game::GenerateOutput()
{
	this->mRenderer->Draw();
}

void Game::LoadData()
{
	if(mCurrentMode == Adventure)
	{
		player = new Player(this, nullptr);
		player->SetPosition(Vector3(-200.0f, 0.0f, 700.0f));

		LevelLoader::Load(this, "Assets/Map/Adventure/Adventure1.json");
	}
	else if(mCurrentMode == Build)
	{
		player = new Player(this, nullptr);
		player->SetPosition(Vector3(0.0f, 0.0f, 700.0f));

		new Arrow(this, NULL);
	}
	else if(mCurrentMode == Maze)
	{	
		LevelLoader::Load(this, mMazeMapName);
		new Arrow(this, NULL);


		BackGroundMuiscName = "Assets/Sounds/Music/" + 
					mMazeMusic[Random::GetIntRange(0, static_cast<int>(mMazeMusic.size()-1))];
	}
	else if(mCurrentMode == Practice)
	{
		LevelLoader::Load(this, "Assets/Map/Gym.json");
	}
	else if(mCurrentMode == Tutorial)
	{
		LevelLoader::Load(this, "Assets/Map/Tutorial.json");
		new Arrow(this, NULL);
	}

	if(BackGroundMuiscName != "Empty" && BackGroundMuiscName != "EMPTY")
	{
		Mix_PlayChannel(mGameMusic, GetSound(BackGroundMuiscName), -1);
	}
}


void Game::LoadAdvanture()
{
	if(!randomMap)
	{
		adventureMapIndex++;
		if(adventureMapIndex >= mAdvantureMap.size())
		{
			adventureMapIndex = static_cast<int>(mAdvantureMap.size())-1;
			randomMap = true;
		}
	}
	else
	{
		adventureMapIndex = Random::GetIntRange(0, static_cast<int>(mAdvantureMap.size()-1));
	}
	
	LevelLoader::Load(this, mAdvantureMap[adventureMapIndex]);
}


void Game::ReadDataFile()
{
	ifstream file("Assets/Data/data.csv");
	string input = "";
	vector<string> str_arr;
	getline(file, input);
	str_arr = CSVHelper::Split(input);
	mAdvantureScore = stoi(str_arr[1]);
}


void Game::WriteDataFile()
{
	ofstream os("Assets/Data/data.csv");
	os<<"mAdventureScore, "<<mAdvantureScore;
	os.close();
}


void Game::LoadAdvantureMaps()
{
	mAdvantureMap.push_back("Assets/Map/Adventure/Adventure1.json");
	mAdvantureMap.push_back("Assets/Map/Adventure/Adventure2.json");
	mAdvantureMap.push_back("Assets/Map/Adventure/Adventure3.json");
	mAdvantureMap.push_back("Assets/Map/Adventure/Adventure4.json");
	mAdvantureMap.push_back("Assets/Map/Adventure/Adventure5.json");
}


void Game::UnloadData()
{
	mFinalBlockPos = Vector3(-500.0f, 0.0f, 0.0f);
	randomMap = false;
	adventureMapIndex = 0;

	while (!mActors.empty())
	{
		delete mActors.back();
	}

	Object.clear();
	cameraArray.clear();
	while(!CheckpointArray.empty()) {CheckpointArray.pop();}

	Mix_HaltChannel(mGameMusic);
}


Mix_Chunk* Game::GetSound(const std::string& fileName)
{
	Mix_Chunk* chunk = nullptr;
	auto iter = mSounds.find(fileName);
	if (iter != mSounds.end())
	{
		chunk = iter->second;
	}
	else
	{
		chunk = Mix_LoadWAV(fileName.c_str());
		if (!chunk)
		{
			SDL_Log("Failed to load sound file %s", fileName.c_str());
			return nullptr;
		}

		mSounds.emplace(fileName, chunk);
	}
	return chunk;
}


void Game::Shutdown()
{	
	WriteDataFile();
	UnloadData();
	UnLoudSound();
	Mix_CloseAudio();
	this->mRenderer->Shutdown();
	delete this->mRenderer;
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		auto iter2 = mActors.end() - 1;
		std::iter_swap(iter, iter2);
		mActors.pop_back();
	}
}


void Game::addToCameraArray(SecurityCamera* e)
{
	cameraArray.push_back(e);
}


void Game::removeFromCameraArray(SecurityCamera* e)
{
	auto iter = std::find(cameraArray.begin(), cameraArray.end(), e);
	if (iter != cameraArray.end())
	{
		auto iter2 = cameraArray.end() - 1;
		std::iter_swap(iter, iter2);
		cameraArray.pop_back();
	}
}


void Game::PushUI(UIScreen* screen)
{
	mUIStack.emplace_back(screen);
}


bool Game::IsFirstUI(UIScreen* screen)
{
	if(!mUIStack.empty())
	{
		return (mUIStack.back() == screen);
	}
    return false;
}


void Game::CloseSettingMenu()
{
	if(mGameState == Menu) 
	{
		for(int i=0; i<mUIStack.size(); i++)
		{
			mUIStack[i]->Close();
		}
		new MenuScreen(this);
	}
}


Font* Game::GetFont(const std::string& fileName)
{
	auto iter = mFonts.find(fileName);
	if (iter != mFonts.end())
	{
		return iter->second;
	}
	else
	{
		Font* font = new Font(this);
		if (font->Load(fileName))
		{
			mFonts.emplace(fileName, font);
		}
		else
		{
			font->Unload();
			delete font;
			font = nullptr;
		}
		return font;
	}
}


void Game::LoadText(const std::string& fileName)
{
	// Clear the existing map, if already loaded
	mText.clear();
	// Try to open the file
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("Text file %s not found", fileName.c_str());
		return;
	}
	// Read the entire file to a string stream
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	// Open this file in rapidJSON
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);
	if (!doc.IsObject())
	{
		SDL_Log("Text file %s is not valid JSON", fileName.c_str());
		return;
	}
	// Parse the text map
	const rapidjson::Value& actions = doc["TextMap"];
	for (rapidjson::Value::ConstMemberIterator itr = actions.MemberBegin();
		itr != actions.MemberEnd(); ++itr)
	{
		if (itr->name.IsString() && itr->value.IsString())
		{
			mText.emplace(itr->name.GetString(), 
				itr->value.GetString());
		}
	}
}


const std::string& Game::GetText(const std::string& key)
{
	static std::string errorMsg("default");
	// Find this text in the map, if it exists
	auto iter = mText.find(key);
	if (iter != mText.end())
	{
		return iter->second;
	}
	else
	{
		return errorMsg;
	}
}


void Game::LoudSound()
{
	mMenuSound = 0;
	mGameMusic = 1;
	mRunningSound = 2;
	mJumpSound = 3;
	mSlideSound = 4;
	mLandSound = 5;
	mLaserSound = 6;
	mCoinSound = 7; 
	mCheckPointSound = 8;
	mCameraCatchSound = 9;

	mSoundNum = 10;

	mGameVolume = 80;
	SetVolume(mGameVolume);

	mSoundOn = true;
	mMusicOn = true;
}


void Game::UnLoudSound()
{
	for (auto s : mSounds)
	{
		Mix_FreeChunk(s.second);
	}

	mSounds.clear();
}


void Game::ResumeGameSound()
{
	for(int i=1; i<mSoundNum; i++)
	{
		Mix_Resume(i);
	}
}


void Game::PauseGameSound()
{
	for(int i=1; i<mSoundNum; i++)
	{
		Mix_Pause(i);
	}
}


void Game::OpenAllSound()
{
	mSoundOn = true;
	for(int i=0; i<mSoundNum; i++)
	{
		if(i == mGameMusic)
		{
			continue;
		}
		Mix_Volume(i, mGameVolume);
	}
}


void Game::CloseAllSound()
{
	mSoundOn = false;
	for(int i=0; i<mSoundNum; i++)
	{
		if(i == mGameMusic)
		{
			continue;
		}
		Mix_Volume(i, 0);
	}
}


void Game::OpenAllMusic()
{
    mMusicOn = true;
	Mix_Volume(mGameMusic, mGameVolume);
}


void Game::CloseAllMusic()
{
    mMusicOn = false;
	Mix_Volume(mGameMusic, 0);
}


void Game::SetVolume(int volume)
{
	if(mSoundOn)
	{
		for(int i=0; i<mSoundNum; i++)
		{
			if(i == mGameMusic){continue;}

			if(i == mMenuSound)
			{
	            Mix_Volume(i, volume/2);
			}
			else
			{
				Mix_Volume(i, volume);
			}
		}
	}
	
	if(mMusicOn)
	{
		Mix_Volume(mGameMusic, volume/3);
	}

	mGameVolume = volume;
}


void Game::ListFileName(const char *path, int level, vector<string> &strvec)
{
	struct dirent* ent = NULL;
	DIR *pDir;
	
	pDir = opendir(path);
	if (pDir == NULL)
	{
		return;
	}
	while (NULL != (ent = readdir(pDir)))
	{
		if (ent->d_type == 8)
		{
			strvec.push_back(ent->d_name);
		}
		else
		{
			if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
			{	
			continue;
			}
	
		string _path(path);
		string _dirName(ent->d_name);
		string fullDirPath = _path + "/" + _dirName;
		ListFileName(fullDirPath.c_str(), level + 1, strvec);
		}
	}
}


void Game::SetMazeMap(string m) 
{
	string::size_type idx;
	idx = m.find("DIY");
	if(idx == string::npos ) {mMazeMapName = "Assets/Map/Maze/" + m + ".json";}
	else {mMazeMapName = "Assets/Map/Maze/zzDIY/" + m + ".json";}
}





