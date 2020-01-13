#include "PlayerMoveBuild.h"
#include "Player.h"
#include "Game.h"
#include "Renderer.h"
#include "Random.h"
#include "Block.h"
#include "Arrow.h"
#include "Coin.h"
#include "LaserMine.h"
#include "LaserComponent.h"
#include "SecurityCamera.h"
#include "CameraComponent.h"
#include "Checkpoint.h"
#include "HUDBuild.h"
#include "SegmentCast.h"
#include "MeshComponent.h"
#include "BuildPauseScreen.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

const float MaxSpeed_Standing = 400.0f;
const float MaxSpeed_Squating = 200.0f;
const float MaxSpeed_Flying = 1000.0f;
const float MaxSpeed_Balance = 100.0f;

const float Speed_Standing = 700.0f;
const float Speed_Squating = 300.0f;
const float Speed_Balance = 100.0f;
const float Speed_Flying = 5000.0f;

const float MaxJumpTime = 2.0f;

const float mSlideForce = 200.0f;

const float BlockSpeed = 100.0f;
const float BlockScaleChange = 100.0f;
const float QuaternionSpeed = 0.5f;
const float MoveSpeed = 30.0f;

const float Singal = 35.0f;

struct GreaterScale
{
	bool operator()(Actor* b1, Actor* b2)
	{
		float s1 = b1->GetScale();
		float s2 = b2->GetScale();
		if(s1<s2) {return true;}
		else if(s1>s2) {return false;}
		else 
		{
			return b1->GetPosition().x < b2->GetPosition().x;
		}
	}
} mScaleComparator;

PlayerMoveBuild::PlayerMoveBuild(Actor* owner): MoveComponent(owner)
{
	ChangeState(PlayerMoveBuild::Flying);
	mOwner->GetComponent<CameraComponent>()->SetAngleLimit(Math::PiOver2 / 1.1f);

	mRunningSFX = Mix_PlayChannel(mOwner->GetGame()->mRunningSound, mOwner->GetGame()->GetSound("Assets/Sounds/Running.wav"), -1);
	Mix_Pause(mRunningSFX);

	input = PlayerMoveBuild::Blank;

	mLastLandPos = mOwner->GetPosition();

	mCurrentBlock = nullptr;

	GenerateBlocks(5, 5, mDefaultBlockScale, 7);

	vector<string> DIYMaps;
	mOwner->GetGame()->ListFileName("Assets/Map/Maze/zzDIY", 0, DIYMaps);
	mMapFileName = "DIY" + to_string(DIYMaps.size()+2);

	mStartPoint = nullptr;
	mFinalPoint = nullptr;
}


PlayerMoveBuild::~PlayerMoveBuild()
{
	Mix_HaltChannel(mRunningSFX);
}


/* ==========================================  Process Input  =============================================*/

void PlayerMoveBuild::ProcessInput(const Uint8* keyState)
{
	float speed = 0.0f;
	Vector3 forward = Vector3::Zero;
	if(mCurrentState == PlayerMoveBuild::Flying)
	{
		speed = Speed_Flying;
		forward = FlyingForward();

		mPreciseTarget = keyState[SDL_SCANCODE_TAB];

        CreateLaser = keyState[SDL_SCANCODE_1];
		CreateCamera = keyState[SDL_SCANCODE_2];
		CreateMirrorBlock = keyState[SDL_SCANCODE_3];
		CreateCoin = keyState[SDL_SCANCODE_4];
		CreateCheckPoint = keyState[SDL_SCANCODE_5];
		CreateStartPoint = keyState[SDL_SCANCODE_COMMA];
		CreateFinalPoint = keyState[SDL_SCANCODE_PERIOD];
	}
	else
	{
		speed = mSquat?Speed_Squating:Speed_Standing;
		forward = mOwner->GetForward();
	}

	int x, y;
	SDL_GetRelativeMouseState(&x, &y);

	float y_rotation = (static_cast<float>(y)/500.0f) * Math::Pi * 10.0f;
	CameraComponent* camera = mOwner->GetComponent<CameraComponent>();
	camera->SetPitchSpeed(y_rotation);

	if(keyState[SDL_SCANCODE_K])
	{
		if(!this->mKPressed)
		{
			this->mKPressed = true;
			if(mCurrentState == PlayerMoveBuild::Flying)
			{
				ChangeState(PlayerMoveBuild::Falling);
				mOwner->GetComponent<CameraComponent>()->SetAngleLimit(Math::PiOver2 / 2.0f);
			}
			else
			{
				ChangeState(PlayerMoveBuild::Flying);
				mOwner->GetComponent<CameraComponent>()->SetAngleLimit(Math::PiOver2 / 1.1f);
			}
		}
	}
	else
	{
		this->mKPressed = false;
	}

	

	if(mCurrentState == PlayerMoveBuild::Balance)
	{
		if(keyState[SDL_SCANCODE_A])
		{
			mBalanceIndex.x += mBalanceControlRate * mBalanceDirection.x;
			mBalanceIndex.y += mBalanceControlRate * mBalanceDirection.y;
		}
		else if(keyState[SDL_SCANCODE_D])
		{
			mBalanceIndex.x -= mBalanceControlRate * mBalanceDirection.x;
			mBalanceIndex.y -= mBalanceControlRate * mBalanceDirection.y;
		}

		if(keyState[SDL_SCANCODE_W])
		{
			AddForce(forward * speed);
		}		
	}
	else 
	{
		if(!keyState[SDL_SCANCODE_W] && !keyState[SDL_SCANCODE_LCTRL] && !keyState[SDL_SCANCODE_LSHIFT])
		{
			input = PlayerMoveBuild::Blank;
		}

		if(keyState[SDL_SCANCODE_W])
		{
			mWPressed = true;

			if(mCtrlIsPressing)
			{
				if(input == PlayerMoveBuild::W && mCurrentState == PlayerMoveBuild::OnGround && !DisableSquat)
				{
					mOwner->GetComponent<CollisionComponent>()->SetSize(50.0f, 125.0f, 50.0f);

					mSliding = true;
					squatLand = true;
					DisableSquat = true;

					AddForce(mGravity);

					Mix_PlayChannel(mOwner->GetGame()->mSlideSound, mOwner->GetGame()->GetSound("Assets/Sounds/slide.ogg"), 0);
				}
			}
			else if(keyState[SDL_SCANCODE_LSHIFT] && !DisableFastRun)
			{
				if(input == PlayerMoveBuild::W && mCurrentState == PlayerMoveBuild::OnGround)
				{
					if(!mShiftPressed)
					{
						mFastRunTimer = 0.0f;
						mShiftPressed = true;
					}

					mFastRun = true;

					AddForce(forward * speed * 3);
				}
			}
			else
			{
				mFastRun = false;
				input = PlayerMoveBuild::W;
				AddForce(forward * speed);
				DisableSquat = false;
				if(!keyState[SDL_SCANCODE_LSHIFT])
				{
					DisableFastRun = false;
				}
			}
		} 
		else
		{
			mWPressed = false;

			if(keyState[SDL_SCANCODE_S])
			{
				AddForce(forward * -speed);
			}
			else if(keyState[SDL_SCANCODE_S] && keyState[SDL_SCANCODE_W])
			{
				AddForce(Vector3::Zero);
			}
			else if(!keyState[SDL_SCANCODE_S] && !keyState[SDL_SCANCODE_W])
			{
				AddForce(Vector3::Zero);
			}
		}

		if(mCurrentState == Flying || (mCurrentState == OnGround && !mOnTurnOverBlock))
		{
			if(keyState[SDL_SCANCODE_A])
			{
				AddForce(mOwner->GetRight() * -speed);
			}
			else if(keyState[SDL_SCANCODE_D])
			{
				AddForce(mOwner->GetRight() * speed);
			}
			else if(keyState[SDL_SCANCODE_A] && keyState[SDL_SCANCODE_D])
			{
				AddForce(Vector3::Zero);
			}
			else if(!keyState[SDL_SCANCODE_A] && !keyState[SDL_SCANCODE_D])
			{
				AddForce(Vector3::Zero);
			}
		}

		if(mCurrentState != PlayerMoveBuild::Flying)
		{
			if(keyState[SDL_SCANCODE_SPACE])
			{
				if(!mSpacePressed && mCurrentState == PlayerMoveBuild::OnGround &&
						  !HeightChange && input != PlayerMoveBuild::Shift) 
				{
					Mix_PlayChannel(mOwner->GetGame()->mJumpSound, mOwner->GetGame()->GetSound("Assets/Sounds/Jump.wav"), 0);
					mSpacePressed = true;
					AddForce(mJumpForce);
					ChangeState(PlayerMoveBuild::Jump);

					if(mFastRun)
					{
						mFastJump = true;
					}
				}
			}
			else
			{
				mSpacePressed = false;
			}

			if(keyState[SDL_SCANCODE_LCTRL])
			{
				mCtrlIsPressing = true;

				if(mWPressed)
				{
					if(input == PlayerMoveBuild::Ctrl)
					{
						AddForce(forward * speed);
					}
				}
				else
				{
					input = PlayerMoveBuild::Ctrl;

					if(!mCtrlPressed)
					{
						mSquat = true;
						mCtrlPressed = true;
						mOwner->GetComponent<CollisionComponent>()->SetSize(50.0f, 125.0f, 50.0f);
						squatLand = true;
					}
					HeightChange = false;
				}
			}
			else
			{
				mCtrlIsPressing = false;

				if(mCtrlPressed)
				{
					mFixJumpTimer = -0.3;
					mCtrlPressed = false;
					HeightChange = true;
				}
			}
		}
		else
		{
			if(keyState[SDL_SCANCODE_SPACE])
			{
				AddForce(Vector3::UnitZ * speed);
			}
			else if(keyState[SDL_SCANCODE_LCTRL])
			{
				AddForce(Vector3::UnitZ * -speed);
			}
			else if((keyState[SDL_SCANCODE_SPACE] && keyState[SDL_SCANCODE_LCTRL]) || 
					(!keyState[SDL_SCANCODE_SPACE] && !keyState[SDL_SCANCODE_LCTRL]))
			{
				AddForce(Vector3::Zero);
			}
		}


		if(keyState[SDL_SCANCODE_LSHIFT])
		{
			mShiftIsPressing = true;

			if(input == PlayerMoveBuild::Blank)
			{
				input = PlayerMoveBuild::Shift;
			}
			
			if(keyState[SDL_SCANCODE_SPACE])
			{
				if(!mSpacePressed && mCurrentState == PlayerMoveBuild::OnGround)
				{
					mSpacePressed = true;
					FixJump();
					mFixJumpTimer = -0.3;
				}
			}
			else
			{
				mSpacePressed = false;
			}
		}
		else
		{
			mShiftIsPressing = false;

			if(mShiftPressed)
			{
				mShiftPressed = false;
			}
		}


		if(keyState[SDL_SCANCODE_J])
		{
			if(!this->mJPressed)
			{
				this->mJPressed = true;
				if(superjump)
				{
					superjump = false;
					mJumpForce = NormalJumpForce;
				}
				else
				{
					superjump = true;
					mJumpForce = SuperJumpForce;
				}
			}
		}
		else
		{
			this->mJPressed = false;
		}

		float x_rotation = (static_cast<float>(x)/500.0f) * Math::Pi * 10.0f;
		SetAngularSpeed(x_rotation);
	}

	if(mCurrentState == Flying)
	{
		if(keyState[SDL_SCANCODE_LEFT] && !keyState[SDL_SCANCODE_RIGHT])
		{
			if(keyState[SDL_SCANCODE_X]) {ObjectMoveVelocity.y = -MoveSpeed;}
			else {mBlockVelocity.y = -BlockSpeed;}
		}
		else if(keyState[SDL_SCANCODE_RIGHT] && !keyState[SDL_SCANCODE_LEFT])
		{
			if(keyState[SDL_SCANCODE_X]) {ObjectMoveVelocity.y = MoveSpeed;}
			else {mBlockVelocity.y = BlockSpeed;}
		}
		else
		{
			mBlockVelocity.y = 0.0f;
			ObjectMoveVelocity.y = 0.0f;
		}

		if(keyState[SDL_SCANCODE_UP] && !keyState[SDL_SCANCODE_DOWN])
		{
			if(keyState[SDL_SCANCODE_X]) {ObjectMoveVelocity.x = MoveSpeed;}
			else {mBlockVelocity.x = BlockSpeed;}
		}
		else if(keyState[SDL_SCANCODE_DOWN] && !keyState[SDL_SCANCODE_UP])
		{
			if(keyState[SDL_SCANCODE_X]) {ObjectMoveVelocity.x = -MoveSpeed;}
			else {mBlockVelocity.x = -BlockSpeed;}
		}
		else
		{
			mBlockVelocity.x = 0.0f;
			ObjectMoveVelocity.x = 0.0f;
		}

		if(keyState[SDL_SCANCODE_U] && !keyState[SDL_SCANCODE_I])
		{
			if(keyState[SDL_SCANCODE_X]) {ObjectMoveVelocity.z = MoveSpeed;}
			else {mBlockVelocity.z = BlockSpeed;}
		}
		else if(keyState[SDL_SCANCODE_I] && !keyState[SDL_SCANCODE_U])
		{
			if(keyState[SDL_SCANCODE_X]) {ObjectMoveVelocity.z = -MoveSpeed;}
			else {mBlockVelocity.z = -BlockSpeed;}
		}
		else
		{
			mBlockVelocity.z = 0.0f;
			ObjectMoveVelocity.z = 0.0f;
		}

		if(keyState[SDL_SCANCODE_EQUALS] && !keyState[SDL_SCANCODE_MINUS])
		{
			mBlockScaleSpeed = BlockScaleChange;
		}
		else if(keyState[SDL_SCANCODE_MINUS] && !keyState[SDL_SCANCODE_EQUALS])
		{
			mBlockScaleSpeed = -BlockScaleChange;
		}
		else
		{
			mBlockScaleSpeed = 0.0f;
		}

		if(keyState[SDL_SCANCODE_KP_4]) // x
		{
			ObjectQuatVelocity.x = keyState[SDL_SCANCODE_X]?-QuaternionSpeed:QuaternionSpeed;
		}
		else
		{
			ObjectQuatVelocity.x = 0.0f;
		}
		if(keyState[SDL_SCANCODE_KP_2]) // y
		{
			ObjectQuatVelocity.y = keyState[SDL_SCANCODE_X]?-QuaternionSpeed:QuaternionSpeed;
		}
		else
		{
			ObjectQuatVelocity.y = 0.0f;
		}
		if(keyState[SDL_SCANCODE_KP_6]) // z
		{
			ObjectQuatVelocity.z = keyState[SDL_SCANCODE_X]?-QuaternionSpeed:QuaternionSpeed;
		}
		else
		{
			ObjectQuatVelocity.z = 0.0f;
		}
		if(keyState[SDL_SCANCODE_KP_8]) // w
		{
			ObjectQuatVelocity.w = keyState[SDL_SCANCODE_X]?-QuaternionSpeed:QuaternionSpeed;
		}
		else
		{
			ObjectQuatVelocity.w = 0.0f;
		}
	}
}


void PlayerMoveBuild::ChangeTexture(int y)
{
	if(mCurrentBlock)
	{
		MeshComponent* mesh = mCurrentBlock->GetComponent<MeshComponent>();
		if(y>0)
		{
			mesh->ChangeTextureIndex(1);
		}
		else
		{
			mesh->ChangeTextureIndex(-1);
		}
		mOwner->GetGame()->player->GetComponent<HUDBuild>()->SetTargetIndex(
			mesh->GetTextureIndex());
	}
}


void PlayerMoveBuild::HandleKeyPress(int key)
{
	if(mCurrentState != Flying){return;}

	CastInfo info = mOwner->GetGame()->player->GetComponent<UIComponent>()->GetCastInfo();
	if(info.mActor)
	{
		if(key == SDL_BUTTON_LEFT)
		{
			if(CreateLaser)
			{
				mObject = new LaserMine(mOwner->GetGame(), nullptr);
				mObject->SetQuaternion(Quaternion(0.0, 0.0, -0.706825, 0.707388));
				mObject->SetPosition(info.mPoint + 10.0f * info.mNormal);
				new LaserComponent(mObject);
			}
			else if(CreateCamera)
			{
				mObject = new SecurityCamera(mOwner->GetGame(), nullptr);
				mObject->SetPosition(info.mPoint + 10.0f * info.mNormal);
				mObject->SetQuaternion(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
			}
			else if(CreateMirrorBlock)
			{
				mObject = new Block(mOwner->GetGame(), nullptr);
				mObject->SetPosition(info.mPoint + 7.5f * info.mNormal);
				mObject->SetScale(15.0f);
				mObject->GetComponent<MeshComponent>()->SetTextureIndex(10);
				mObject->SetMirror(true);
			}
			else if(CreateCoin)
			{
				mObject = new Coin(mOwner->GetGame(), nullptr);
				Vector3 pos;
				if(mPreciseTarget) {pos = info.mPoint; pos.z += 100.0f;}
				else {pos = info.mActor->GetPosition(); pos.z += info.mActor->GetScale()/2 + 100.0f;}
				mObject->SetPosition(pos);
				mObject->SetScale(0.5f);
			}
			else if(CreateCheckPoint)
			{
				mObject = new Checkpoint(mOwner->GetGame(), nullptr);
				Vector3 pos;
				if(mPreciseTarget) {pos = info.mPoint; pos.z += 100.0f;}
				else {pos = info.mActor->GetPosition(); pos.z += info.mActor->GetScale()/2 + 100.0f;}
				mObject->SetPosition(pos);
			}
			else if(CreateStartPoint)
			{
				if(!mStartPoint)
				{
					mStartPoint = new Checkpoint(mOwner->GetGame(), nullptr);
				}
				Vector3 pos = info.mActor->GetPosition();
				pos.z += info.mActor->GetScale()/2 + 100.0f;
				mStartPoint->SetPosition(pos);
				mStartPoint->GetComponent<MeshComponent>()->SetTextureIndex(2);
			}
			else if(CreateFinalPoint)
			{
				if(!mFinalPoint)
				{
					mFinalPoint = new Checkpoint(mOwner->GetGame(), nullptr);
				}
				Vector3 pos = info.mActor->GetPosition();
				pos.z += info.mActor->GetScale()/2 + 100.0f;
				mFinalPoint->SetPosition(pos);
				mFinalPoint->GetComponent<MeshComponent>()->SetTextureIndex(0);
			}
			else
			{
				Block* b = new Block(mOwner->GetGame(), nullptr);
				if(mPreciseTarget)
				{
					b->SetPosition(info.mPoint + mDefaultBlockScale/2 * info.mNormal);
				}
				else
				{
					b->SetPosition(info.mActor->GetPosition()+
						((info.mActor->GetScale()+mDefaultBlockScale)/2)*info.mNormal);
				}
				b->SetScale(mDefaultBlockScale);
				b->GetComponent<MeshComponent>()->SetTextureIndex(mDefaultBlockTexIndex);
				b->SetRotation(mDefaultBlockRotation);
			}
		}
		else if(key == SDL_BUTTON_RIGHT)
		{
			if(CreateLaser || CreateCamera)
			{
				if(mObject)
				{
					mObject->SetState(ActorState::Destroy);	
					mObject = nullptr;
				}
			}
			else
			{
				mLastBlockStack.push(
				info.mActor->GetPosition(), 
				info.mActor->GetScale(),
				mOwner->GetGame()->player->GetComponent<HUDBuild>()->GetTargetIndex(),
				info.mActor->GetRotation());

				if(info.mActor->GetMirror()) {mObject = nullptr;}

				if(info.mActor == mCurrentBlock)
				{
					mCurrentBlock = nullptr;
				}

				info.mActor->SetState(ActorState::Destroy);	
		
				mOwner->GetGame()->player->GetComponent<HUDBuild>()->BreakBlock();
			}
		}
		else if(key == SDL_BUTTON_MIDDLE)
		{
			mCurrentBlock = info.mActor;
			mDefaultBlockScale = mCurrentBlock->GetScale();
			mDefaultBlockTexIndex = mOwner->GetGame()->player->GetComponent<HUDBuild>()->GetTargetIndex();
			mDefaultBlockRotation = mCurrentBlock->GetRotation();
		}	
	}

	if(key == SDLK_l)
	{
		cout<<"successful store"<<Random::GetIntRange(0, 9)<<endl;
		WriteBlockToFile();
	}
	else if(key == SDLK_m)
	{
		if(mCurrentBlock)
		{
			mDefaultBlockTexIndex = mOwner->GetGame()->player->GetComponent<HUDBuild>()->GetTargetIndex();
			mDefaultBlockScale = mCurrentBlock->GetScale();
		}
	}
	else if(key == SDLK_n)
	{
		Block* b = new Block(mOwner->GetGame(), nullptr);
		Vector3 pos = mOwner->GetPosition();
		pos.x += mOwner->GetForward().x * 1000.0f;
		pos.y += mOwner->GetForward().y * 1000.0f;
		b->SetPosition(pos);
		b->SetScale(mDefaultBlockScale);
		b->GetComponent<MeshComponent>()->SetTextureIndex(mDefaultBlockTexIndex);
	}
	else if(key == SDLK_o)
	{
		if(mCurrentBlock)
		{
			mCurrentBlock->SetScale(Singal);
			mCurrentBlock->GetComponent<MeshComponent>()->SetTextureIndex(0);
		}
	}
	else if(key == SDLK_p)
	{
		Vector3 pp = mOwner->GetPosition();
		cout<<"Player: "; pp.Print();
		if(info.mActor)
		{
			Vector3 cp = info.mActor->GetPosition();
			cout<<"Block: "; cp.Print();
		}
	}
	else if(key == SDLK_v)
	{
		ClearAllBlocks();
	}
	else if(key == SDLK_y)
	{
		if(mCurrentBlock)
		{
			mCurrentBlock->SetRotation(mCurrentBlock->GetRotation() + Math::PiOver2);
		}
	}
	else if(key == SDLK_z)
	{
		if(!mLastBlockStack.empty())
		{
			BlockInfo bi = mLastBlockStack.top();
			Block* b = new Block(mOwner->GetGame(), nullptr);
			b->SetPosition(bi.pos);
			b->SetScale(bi.scale);
			b->GetComponent<MeshComponent>()->SetTextureIndex(bi.index);
			b->SetRotation(bi.rotation);
		}
	}
	else if(key == SDLK_ESCAPE)
	{
		new BuildPauseScreen(mOwner->GetGame());
	}
	
	if(mObject)
	{
		LaserMine* l = dynamic_cast<LaserMine*>(mObject);
		SecurityCamera* s = dynamic_cast<SecurityCamera*>(mObject);
		Block* b = dynamic_cast<Block*>(mObject);
		if(key == SDLK_KP_1) // startPos
		{
			if(l || b) {mObject->SetStartPos();}
		}
		else if(key == SDLK_KP_3) // endPos
		{
			if(l || b) {mObject->SetEndPos();}
		}
		else if(key == SDLK_KP_7) // startQuat
		{
			if(l || b) {mObject->SetStartQuat();}
			else if(s) {s->SetStartQ();}
		}
		else if(key == SDLK_KP_9) // endQuat
		{
			if(l || b) {mObject->SetEndQuat();}
			else if(s) {s->SetEndQ();}
		}
	}
}


void PlayerMoveBuild::UpdateBlock(float deltaTime)
{
	if(mCurrentBlock)
	{
		Vector3 pos = mCurrentBlock->GetPosition();
		pos += deltaTime * mBlockVelocity;

		bool x1 = false;
		bool x2 = false;
		bool y1 = false;
		bool y2 = false;
		bool z1 = false;
		bool z2 = false;

		CollisionComponent* mBlockCollision = mCurrentBlock->GetComponent<CollisionComponent>();
		vector<Actor*> blocks = mOwner->GetGame()->Object;
		Vector3 TotalOffset = Vector3::Zero;
		for(auto b: blocks)
		{
			if(mCurrentBlock == b)
			{
				continue;
			}

			CollisionComponent* bCollision = b->GetComponent<CollisionComponent>();
			Vector3 offset = Vector3::Zero;
			CollSide side = mBlockCollision->GetMinOverlap(bCollision, offset);

			if(side != CollSide::None)
			{
				TotalOffset += offset;

				switch (side)
				{
					case CollSide::SideX1:
						x1 = true;
						break;
					case CollSide::SideX2:
						x2 = true;
						break;
					case CollSide::SideY1:
						y1 = true;
						break;
					case CollSide::SideY2:
						y2 = true;
						break;
					case CollSide::Top:
						z1 = true;
						break;
					case CollSide::Bottom:
						z2 = true;
						break;	
					default:
						break;
				}
			}
		}

		pos += TotalOffset;
		mCurrentBlock->SetPosition(pos);
		
		float scale = mCurrentBlock->GetScale();
		if((x1&&x2) || (y1&&y2) || (z1&&z2))
		{
			if(mBlockScaleSpeed > 0.0f)
			{
				mBlockScaleSpeed = 0.0f;
			}
		}
		scale += deltaTime * mBlockScaleSpeed;
		if(scale < 0.0f)
		{
			scale = 0.0f;
		}
		mCurrentBlock->SetScale(scale);
	}
}


void PlayerMoveBuild::UpdateObject(float deltaTime)
{
	if(mObject)
	{
		Quaternion quat = mObject->GetQuaternion();
		quat += deltaTime * ObjectQuatVelocity;
		quat.Normalize();
		mObject->SetQuaternion(quat);
		
		Vector3 pos = mObject->GetPosition();
		pos += deltaTime * ObjectMoveVelocity;
		mObject->SetPosition(pos);
	}
}

/* ==========================================  Update Functions  =============================================*/

void PlayerMoveBuild::Update(float deltaTime)
{
	touchBottom = false;

	if(mCurrentState == PlayerMoveBuild::OnGround)
	{
		UpdateOnGround(deltaTime);
	}
	else if(mCurrentState == PlayerMoveBuild::Jump)
	{
		UpdateJump(deltaTime);
	}
	else if(mCurrentState == PlayerMoveBuild::Falling)
	{
		UpdateFalling(deltaTime);
	}
	else if(mCurrentState == PlayerMoveBuild::WallClimb)
	{
		UpdateWallClimb(deltaTime);
	}
	else if(mCurrentState == PlayerMoveBuild::WallRun)
	{
		UpdateWallRun(deltaTime);
	}
	else if(mCurrentState == PlayerMoveBuild::Flying)
	{
		UpdateFlying(deltaTime);
	}
	else if(mCurrentState == PlayerMoveBuild::Sliding)
	{
		UpdateSliding(deltaTime);
	}
	else if(mCurrentState == PlayerMoveBuild::Balance)
	{
		UpdateBalance(deltaTime);
	}

// Check Sound
	if(
	   ((mCurrentState == PlayerMoveBuild::OnGround && mVelocity.Length() >= 50.0f) && !mSquat)||
		mCurrentState == PlayerMoveBuild::WallClimb ||
		mCurrentState == PlayerMoveBuild::WallRun)
	{
		Mix_Resume(mRunningSFX);
	}
	else
	{
		Mix_Pause(mRunningSFX);
	}

// Respawn
	if(mCurrentState != Flying)
	{
		if(mOwner->GetPosition().z <= mOwner->GetGame()->GetFinalPos().z-750.0f)
		{
			Respawn();
		}
	}

// Update mCurrentBlock
	UpdateBlock(deltaTime);

// Update mObject
	UpdateObject(deltaTime);

// Update TurnOver
	if(mOnTurning)
	{
		if(mTurnOverTimer <= mTurnOverTime)
		{
			mTurnOverTimer += deltaTime;
			if(mTurnOverTimer >= 0.0f)
			{
				float rotation = Math::Lerp(mStartRotation, mEndRotation, mTurnOverTimer/mTurnOverTime);
				mOwner->SetRotation(rotation);
			}
		}
		else
		{
			mOwner->SetRotation(mEndRotation);
			mTurnOverTimer = -0.1f;
			mOnTurning = false;
		}
	}

// Test
	UpdatePrint(deltaTime);
}


void PlayerMoveBuild::UpdatePrint(float deltaTime)
{
	
}


void PlayerMoveBuild::PhysicsUpdate(float deltaTime)
{
	mAcceleration = mPendingForces * (1.0f / mMass);

	mVelocity += mAcceleration * deltaTime;

	if(mCurrentState == PlayerMoveBuild::Flying)
	{
		FixXYZVelocity();
	}
	else
	{
		FixXYVelocity();
	}

	Vector3 pos = mOwner->GetPosition();
	pos += mVelocity * deltaTime;
	mOwner->SetPosition(pos);

	mOwner->SetRotation(mOwner->GetRotation() + deltaTime * GetAngularSpeed());

	mPendingForces = Vector3::Zero;
}


void PlayerMoveBuild::UpdateSliding(float deltaTime)
{
	PhysicsUpdate(deltaTime);

	mSlideTimer += deltaTime;

	if(mSlideTimer > 0.4f)
	{
		ChangeState(PlayerMoveBuild::OnGround);
		mSlideTimer = 0.0f;
		HeightChange = true;
		mSliding = false;
	}

	Vector3 pos = mOwner->GetPosition();
	vector<Actor*> blocks = mOwner->GetGame()->Object;
	CollisionComponent* playerCollision = mOwner->GetComponent<CollisionComponent>();

	for(auto block: blocks)
	{
		Vector3 dist = pos - block->GetPosition();
		if(dist.Length() > 1000) {continue;}

		CollisionComponent* blockCollision = block->GetComponent<CollisionComponent>();
		FixCollision(playerCollision, blockCollision);
	}

	AddForce(mOwner->GetForward() * mSlideForce);
}


void PlayerMoveBuild::UpdateFlying(float deltaTime)
{
	PhysicsUpdate(deltaTime);
}


void PlayerMoveBuild::UpdateOnGround(float deltaTime)
{
	PhysicsUpdate(deltaTime);

	bool falling = true;
	bool touchTurnOverBlock = false;

	Vector3 pos = mOwner->GetPosition();
	vector<Actor*> blocks = mOwner->GetGame()->Object;
	CollisionComponent* playerCollision = mOwner->GetComponent<CollisionComponent>();
	for(auto b: blocks)
	{
		Block* block = static_cast<Block*>(b);
		Vector3 dist = pos - block->GetPosition();

		if(dist.Length() > 2000) {continue;}
	
		CollisionComponent* blockCollision = block->GetComponent<CollisionComponent>();
		CollSide side = FixCollision(playerCollision, blockCollision);
		if(side == CollSide::Top)
		{	
			int balance = block->GetBalance();
			if(balance != 0)
			{
				SetUpBalance(balance);
				ChangeState(PlayerMoveBuild::Balance);
			}

			int TurnOverDir = block->GetTurnOver();

			if(TurnOverDir != 0)
			{
				touchTurnOverBlock = true;
				if(!mOnTurning && !mOnTurnOverBlock)
				{
					mOnTurning = true;
					mOnTurnOverBlock = true;
					mStartRotation = mOwner->GetRotation();
					mEndRotation = mStartRotation + TurnOverDir * Math::PiOver2;
				}
			}
			
			mLastLandPos = block->GetPosition();
			mLastLandPos.z += (block->GetScale())*0.5f;
			falling = false;
		}
		else if(CollisionComponent::IsSide(side))
		{
			if(CanWallClimb(side))
			{
				ChangeState(WallClimb);
				mWallClimbTimer = 0.0f;
				return;
			}

			if(mSliding)
			{
				mSliding = false;
				mVelocity.x = 0.0f;
				mVelocity.y = 0.0f;
			}
		}
	}
	
	if(falling)
	{
		ChangeState(PlayerMoveBuild::Falling);
	}
	if(!touchTurnOverBlock)
	{
		mOnTurnOverBlock = false;
	}

	UpdateSquating(deltaTime);
	UpdateFixJump(deltaTime);
	UpdateFastRun(deltaTime);
}


void PlayerMoveBuild::UpdateJump(float deltaTime)
{
	AddForce(mGravity);
	
	PhysicsUpdate(deltaTime);

	Vector3 pos = mOwner->GetPosition();
	vector<Actor*> blocks = mOwner->GetGame()->Object;
	CollisionComponent* playerCollision = mOwner->GetComponent<CollisionComponent>();
	for(auto block: blocks)
	{
		Vector3 dist = pos - block->GetPosition();
		if(dist.Length() > 1000) {continue;}

		CollisionComponent* blockCollision = block->GetComponent<CollisionComponent>();
		CollSide side = FixCollision(playerCollision, blockCollision);
		if(side == CollSide::Bottom)
		{
			mVelocity.z = 0.0f;
		}
		else if(CollisionComponent::IsSide(side))
		{
			if(CanWallClimb(side))
			{
				ChangeState(WallClimb);
				mWallClimbTimer = 0.0f;
				return;
			}
			else if(CanWallRun(side))
			{
				ChangeState(WallRun);
				mWallRunTimer = 0.0f;
				return;
			}
		}
	}

	if(mVelocity.z <= 0.0f)
	{
		ChangeState(PlayerMoveBuild::Falling);
	}
}


void PlayerMoveBuild::UpdateFalling(float deltaTime)
{
	AddForce(mGravity);
	
	PhysicsUpdate(deltaTime);

	Vector3 pos = mOwner->GetPosition();
	vector<Actor*> blocks = mOwner->GetGame()->Object;
	CollisionComponent* playerCollision = mOwner->GetComponent<CollisionComponent>();
	for(auto block: blocks)
	{
		Vector3 dist = pos - block->GetPosition();
		if(dist.Length() > 1000) {continue;}

		CollisionComponent* blockCollision = block->GetComponent<CollisionComponent>();
		CollSide side = FixCollision(playerCollision, blockCollision);
		if(side == CollSide::Top)
		{
			mVelocity.z = 0.0f;
			if(!squatLand)
			{
				Mix_PlayChannel(mOwner->GetGame()->mLandSound, mOwner->GetGame()->GetSound("Assets/Sounds/Land.wav"), 0);
			}
			else
			{
				squatLand = false;
			}

			if(mSliding)
			{
				ChangeState(PlayerMoveBuild::Sliding);
			}
			else
			{
				ChangeState(PlayerMoveBuild::OnGround);
			}

			if(mFastJump)
			{
				mFastJump = false;
			}
		}
	}
}


void PlayerMoveBuild::UpdateWallClimb(float deltaTime)
{
	mWallClimbTimer += deltaTime;

	AddForce(mGravity);
	if(mWallClimbTimer <= 0.6f)
	{
		AddForce(mClimbForce);
	}

	PhysicsUpdate(deltaTime);

	Vector3 pos = mOwner->GetPosition();
	vector<Actor*> blocks = mOwner->GetGame()->Object;
	CollisionComponent* playerCollision = mOwner->GetComponent<CollisionComponent>();

	bool collide = false;
	for(auto block: blocks)
	{
		Vector3 dist = pos - block->GetPosition();
		if(dist.Length() > 1000) {continue;}

		CollisionComponent* blockCollision = block->GetComponent<CollisionComponent>();
		CollSide side = FixCollision(playerCollision, blockCollision);
		if(CollisionComponent::IsSide(side))
		{
			collide = true;
		}
	}

	if(!collide || mVelocity.z <= 0.0f)
	{
		mVelocity.z = 0.0f;
		ChangeState(Falling);
	}
}


void PlayerMoveBuild::UpdateWallRun(float deltaTime)
{
	mWallRunTimer += deltaTime;

	CameraComponent* c = mOwner->GetComponent<CameraComponent>();
	c->UpdateAngle(deltaTime, mWallRunMultiplier);

	AddForce(mGravity);
	if(mWallRunTimer <= 0.4f)
	{
		AddForce(mWallRunForce);
	}

	PhysicsUpdate(deltaTime);

	Vector3 pos = mOwner->GetPosition();
	vector<Actor*> blocks = mOwner->GetGame()->Object;
	CollisionComponent* playerCollision = mOwner->GetComponent<CollisionComponent>();
	for(auto block: blocks)
	{
		Vector3 dist = pos - block->GetPosition();
		if(dist.Length() > 1000) {continue;}

		CollisionComponent* blockCollision = block->GetComponent<CollisionComponent>();
		FixCollision(playerCollision, blockCollision);
	}

	if(mVelocity.z <=0.0f)
	{
		ChangeState(Falling);
		c->FixUp();
		mWallRunMultiplier = Vector2::Zero;
	}
}


void PlayerMoveBuild::UpdateBalance(float deltaTime)
{
	PhysicsUpdate(deltaTime);

	bool falling = true;
	
	Vector3 pos = mOwner->GetPosition();
	vector<Actor*> blocks = mOwner->GetGame()->Object;
	CollisionComponent* playerCollision = mOwner->GetComponent<CollisionComponent>();
	for(auto block: blocks)
	{
		Vector3 dist = pos - block->GetPosition();
		if(dist.Length() > 1000) {continue;}
	
		CollisionComponent* blockCollision = block->GetComponent<CollisionComponent>();
		CollSide side = FixCollision(playerCollision, blockCollision);
		if(side == CollSide::Top)
		{
			falling = false;
		}
	}
	
	CameraComponent* camera = mOwner->GetComponent<CameraComponent>();
	if(falling)
	{
		mBalanceIndex = Vector2::Zero;
		ChangeState(PlayerMoveBuild::Falling);
		camera->FixUp();
		camera->SetRotationSpeed(0.8f);
		return;
	}
	
	if(Math::ToDegrees(mOwner->GetRotation()) != mIdealBalanceAngle)
	{
		FixBalanceAngle(deltaTime);
		return;
	}

	AddForce(mOwner->GetForward() * Speed_Balance);
	Vector2 lostBalance = Vector2::Zero;

	if(mBalanceIndex.x == 0.0f && mBalanceIndex.y == 0.0f)
	{
		srand((unsigned)time(0));
		bool b = rand()%2;
		if(b)
		{
			mBalanceIndex.x += mBalanceRate * deltaTime * abs(mBalanceDirection.x);
			mBalanceIndex.y += mBalanceRate * deltaTime * abs(mBalanceDirection.y);
		}
		else
		{
			mBalanceIndex.x += -mBalanceRate * deltaTime * abs(mBalanceDirection.x);
			mBalanceIndex.y += -mBalanceRate * deltaTime * abs(mBalanceDirection.y);
		}
	}
	else if(mBalanceIndex.x < 0.0f || mBalanceIndex.y < 0.0f)
	{
		mBalanceIndex.x += -mBalanceRate * deltaTime * abs(mBalanceDirection.x);
		mBalanceIndex.y += -mBalanceRate * deltaTime * abs(mBalanceDirection.y);

		if(mBalanceIndex.x < -MaxBalanceAngle)
		{
			lostBalance.x = -1;
		}
		else if(mBalanceIndex.y < -MaxBalanceAngle)
		{
			lostBalance.y = -1;
		}
	}
	else if(mBalanceIndex.x > 0.0f || mBalanceIndex.y > 0.0f)
	{
		mBalanceIndex.x += mBalanceRate * deltaTime * abs(mBalanceDirection.x);
		mBalanceIndex.y += mBalanceRate * deltaTime * abs(mBalanceDirection.y);

		if(mBalanceIndex.x > MaxBalanceAngle)
		{
			lostBalance.x = 1;
		}
		else if(mBalanceIndex.y > MaxBalanceAngle)
		{
			lostBalance.y = 1;
		}
	}

	if(lostBalance.x != 0.0f || lostBalance.y != 0.0f)
	{
		mUnderBalance = false;
		AddForce(Vector3(-10000.0f * lostBalance.x, -10000.0f * lostBalance.y, 0.0f));
	}

	camera->SetUp(mBalanceIndex.x, mBalanceIndex.y);
}


/* ==========================================  Helper Functions  =============================================*/

void PlayerMoveBuild::ChangeState(MoveState state)
{
	mCurrentState = state;
}


bool PlayerMoveBuild::CanWallRun(CollSide side)
{
	Vector3 Normal = Vector3::Zero;

	if(side == CollSide::SideX1 || side == CollSide::SideX2)
	{
		Normal = Vector3::UnitX;	
	}
	else if(side == CollSide::SideY1 || side == CollSide::SideY2)
	{
		Normal = Vector3::UnitY;
	}

	float dot1 = Vector3::Dot(mOwner->GetForward(), Normal);
	if(dot1 >= 0.5f || dot1 <= -0.5)
	{
		return false;
	}

	Vector3 VelocityDirection = mVelocity;
	VelocityDirection.Normalize();
	float dot2 = Vector3::Dot(VelocityDirection, mOwner->GetForward());
	dot2 -= 1;
	if(dot2 >= 0.5f || dot2 <= -0.5f)
	{
		return false;
	}

	if(mVelocity.Length() <= 350.0f)
	{
		return false;
	}

// Set RunIndex
	if(side == CollSide::SideY1)
	{
		mWallRunMultiplier.y = -1;
	}
	else if(side == CollSide::SideY2)
	{
		mWallRunMultiplier.y = 1;
	}
	else if(side == CollSide::SideX1)
	{
		mWallRunMultiplier.x = -1;
	}
	else if(side == CollSide::SideX2)
	{
		mWallRunMultiplier.x = 1;
	}

	mOwner->GetComponent<CameraComponent>()->SetRotationSpeed(0.5f);
	return true;
}


bool PlayerMoveBuild::CanWallClimb(CollSide side)
{
	Vector3 Normal = Vector3::Zero;

	if(side == CollSide::SideX1 || side == CollSide::SideX2)
	{
		Normal = Vector3::UnitX;	
	}
	else if(side == CollSide::SideY1 || side == CollSide::SideY2)
	{
		Normal = Vector3::UnitY;
	}

	float dot1 = Vector3::Dot(mOwner->GetForward(), Normal);
	dot1 = abs(dot1);
	dot1 -= 1;
	if(dot1 >= 0.1f || dot1 <= -0.1f)
	{
		return false;
	}

	Vector3 VelocityDirection = mVelocity;
	VelocityDirection.Normalize();
	float dot2 = Vector3::Dot(VelocityDirection, Normal);
	dot2 = abs(dot2);
	dot2 -= 1;
	if(dot2 >= 0.1f || dot2 <= -0.1f)
	{
		return false;
	}

	if(mVelocity.Length() <= 350.0f)
	{
		return false;
	}
	return true;
}


CollSide PlayerMoveBuild::FixCollision(class CollisionComponent* self, class CollisionComponent* block)
{
	Vector3 offset = Vector3::Zero;
	CollSide side = self->GetMinOverlap(block, offset);

	if(side != CollSide::None)
	{
		Vector3 pos = mOwner->GetPosition();
		pos += offset;
		mOwner->SetPosition(pos);

		if(side == CollSide::SideX1)
		{
			AddForce(Vector3(-700.0f, 0.0f, 0.0f));
		}
		else if(side == CollSide::SideX2)
		{
			AddForce(Vector3(700.0f, 0.0f, 0.0f));
		}
		else if(side == CollSide::SideY1)
		{
			AddForce(Vector3(0.0f, -700.0f, 0.0f));
		}
		else if(side == CollSide::SideY2)
		{
			AddForce(Vector3(0.0f, 700.0f, 0.0f));
		}

		if(HeightChange)
		{
			if(side == CollSide::Bottom)
			{
				touchBottom = true;
				HeightChangeStop = true;
			}
		}
		
	}

	return side;
}


void PlayerMoveBuild::AddForce(const Vector3& force)
{
	mPendingForces += force;
}


void PlayerMoveBuild::FixXYVelocity()
{
	Vector2 xyVelocity = Vector2(mVelocity.x, mVelocity.y);
	
	float MaxSpeed = 0.0f;

	if(mSliding)
	{
		MaxSpeed = MaxSpeed_Flying * 2;
	}
	else if(mSquat)
	{
		MaxSpeed = MaxSpeed_Squating;
	}
	else if(mCurrentState == PlayerMoveBuild::Balance && mUnderBalance)
	{
		MaxSpeed = MaxSpeed_Balance;
	}
	else if(mFastRun || mFastJump)
	{
		MaxSpeed = MaxSpeed_Standing * 3;
	}
	else
	{
		MaxSpeed = MaxSpeed_Standing;
	}

	if(xyVelocity.Length() >= MaxSpeed)
	{
		xyVelocity.Normalize();
		xyVelocity *= MaxSpeed;
	}

	if(mCurrentState == PlayerMoveBuild::OnGround || 
	   mCurrentState == PlayerMoveBuild::WallClimb || 
	   mCurrentState == PlayerMoveBuild::Balance)
	{
		if(Math::NearZero(mAcceleration.x) || 
			(xyVelocity.x > 0 && mAcceleration.x < 0) || (xyVelocity.x < 0 && mAcceleration.x > 0))
		{
			xyVelocity.x *= 0.9f;
		}

		if(Math::NearZero(mAcceleration.y) || 
			(xyVelocity.y > 0 && mAcceleration.y < 0) || (xyVelocity.y < 0 && mAcceleration.y > 0))
		{
			xyVelocity.y *= 0.9f;
		}
	}

	mVelocity.x = xyVelocity.x;
	mVelocity.y = xyVelocity.y;
}


void PlayerMoveBuild::FixXYZVelocity()
{
	Vector3 xyzVelocity = mVelocity;
	
	float MaxSpeed = MaxSpeed_Flying;

	if(xyzVelocity.Length() >= MaxSpeed)
	{
		xyzVelocity.Normalize();
		xyzVelocity *= MaxSpeed;
	}

	if(Math::NearZero(mAcceleration.x) || 
		(xyzVelocity.x > 0 && mAcceleration.x < 0) || (xyzVelocity.x < 0 && mAcceleration.x > 0))
	{
		xyzVelocity.x *= 0.9f;
	}

	if(Math::NearZero(mAcceleration.y) || 
		(xyzVelocity.y > 0 && mAcceleration.y < 0) || (xyzVelocity.y < 0 && mAcceleration.y > 0))
	{
		xyzVelocity.y *= 0.9f;
	}

	if(Math::NearZero(mAcceleration.z) || 
		(xyzVelocity.z > 0 && mAcceleration.z < 0) || (xyzVelocity.z < 0 && mAcceleration.z > 0))
	{
		xyzVelocity.z *= 0.9f;
	}

	mVelocity = xyzVelocity;
}


void PlayerMoveBuild::Respawn()
{
	Vector3 respawn = mOwner->GetComponent<PlayerMoveBuild>()->GetLastLandPos();
	respawn.z += 200.0f;
	mOwner->SetPosition(respawn);
	mOwner->SetRotation(0.0f);
	mVelocity = Vector3::Zero;
	mPendingForces = Vector3::Zero;
	ChangeState(PlayerMoveBuild::Falling);
}


void PlayerMoveBuild::FixJump()
{
	if(mFixJumpTimer < 0.0f)
	{
		mFixJumpTimer = 0.0f;
	}
	cout<<"jump "<<mFixJumpTimer/MaxJumpTime<<endl;
	cout<<endl;
	float XYjumpForce = 30000.0f + Math::Lerp(0, 110000.0f, mFixJumpTimer/MaxJumpTime);
	float ZjumpForce = Math::Lerp(25000, 40000.0f, mFixJumpTimer/MaxJumpTime);
	Vector3 jumpForward = Vector3(mOwner->GetForward().x*XYjumpForce, mOwner->GetForward().y*XYjumpForce, ZjumpForce);
	AddForce(jumpForward);
	ChangeState(PlayerMoveBuild::Jump);
}


Vector3 PlayerMoveBuild::FlyingForward()
{
	Vector3 forward = mOwner->GetForward();
	forward.z = -Math::Sin(mOwner->GetComponent<CameraComponent>()->GetPitchAngle());
	return forward;
}


void PlayerMoveBuild::UpdateSquating(float deltaTime)
{
	if(!touchBottom && !(mCurrentState == PlayerMoveBuild::Sliding))
	{
		HeightChangeStop = false;
	}

	if(HeightChangeStop)
	{
		mOwner->GetComponent<CollisionComponent>()->SetSize(50.0f, 125.0f, 50.0f);
	}

	if(HeightChange && !HeightChangeStop)
	{
		HeightChangeTimer += deltaTime;

		float third_para = 0.0f;
		if(HeightChangeTimer>0.3f)
		{
			third_para = 1.0f;
			HeightChange = false;
			HeightChangeStop = false;
			HeightChangeTimer = 0.0f;
			mSquat = false;
		}
		else
		{
			third_para = HeightChangeTimer/0.3f;
		}

		height = Math::Lerp(125.0, 250.0, third_para);
		mOwner->GetComponent<CollisionComponent>()->SetSize(50.0f, height, 50.0f);
	}
}


void PlayerMoveBuild::UpdateFixJump(float deltaTime)
{
	if(mShiftIsPressing)
	{
		mFixJumpTimer += deltaTime;
		if(mFixJumpTimer > MaxJumpTime)
		{
			mFixJumpTimer = MaxJumpTime;
		}
	}
}


void PlayerMoveBuild::UpdateFastRun(float deltaTime)
{
	if(mFastRun)
	{
		mFastRunTimer += deltaTime;
		if(mFastRunTimer > 1.0f)
		{
			mFastRun = false;
			mFastRunTimer = 0.0f;
			DisableFastRun = true;
		}
	}
}


void PlayerMoveBuild::SetUpBalance(int balance)
{
	mUnderBalance = true;

	Vector3 forward = mOwner->GetForward();

	if(balance == 1)
	{
		if(forward.x > 0)
		{
			mIdealBalanceAngle = 0.0f;
			mBalanceDirection = Vector2(0, 1);
	
			if(forward.y > 0)
			{
				mBalanceAngleMultiplier = -1;
			}
			else if(forward.y < 0)
			{
				mBalanceAngleMultiplier = 1;
			}
			else
			{
				mBalanceAngleMultiplier = 0;
			}
		}
		else
		{
			mIdealBalanceAngle = 180.0f;
			mBalanceDirection = Vector2(0, -1);
	
			if(forward.y > 0)
			{
				mBalanceAngleMultiplier = 1;
			}
			else if(forward.y < 0)
			{
				mBalanceAngleMultiplier = -1;
			}
			else
			{
				mBalanceAngleMultiplier = 0;
			}
		}
	}
	else if(balance == -1)
	{
		if(forward.y > 0)
		{
			mIdealBalanceAngle = 90.0f;
			mBalanceDirection = Vector2(-1, 0);
	
			if(forward.x > 0)
			{
				mBalanceAngleMultiplier = 1;
			}
			else if(forward.x < 0)
			{
				mBalanceAngleMultiplier = -1;
			}
			else
			{
				mBalanceAngleMultiplier = 0;
			}
		}
		else
		{
			mIdealBalanceAngle = 270.0f;
			mBalanceDirection = Vector2(1, 0);
	
			if(forward.x > 0)
			{
				mBalanceAngleMultiplier = -1;
			}
			else if(forward.x < 0)
			{
				mBalanceAngleMultiplier = 1;
			}
			else
			{
				mBalanceAngleMultiplier = 0;
			}
		}
	}
}


void PlayerMoveBuild::FixBalanceAngle(float deltaTime)
{
	if(mBalanceAngleMultiplier == 0)
		return;

	float angle = Math::ToDegrees(mOwner->GetRotation());
	
	if(mBalanceAngleMultiplier > 0)
	{
		angle += mBalanceAngleRate * deltaTime;
		if(angle > mIdealBalanceAngle)
		{
			angle = mIdealBalanceAngle;
			mBalanceAngleMultiplier = 0;
		}
	}
	else if(mBalanceAngleMultiplier < 0)
	{
		angle -= mBalanceAngleRate * deltaTime;
		if(angle < mIdealBalanceAngle)
		{
			angle = mIdealBalanceAngle;
			mBalanceAngleMultiplier = 0;
		}
	}
	mOwner->SetRotation(Math::ToRadians(angle));
}


void PlayerMoveBuild::GenerateBlocks(int row, int col, float scale, int texIndex)
{
	float x = 0.0f;
	for(int i=0; i<row; i++)
	{
		if(col%2 == 0)
		{
			float y = scale/2;

			for(int j=0; j<(col/2); j++)
			{
				Block* rightB = new Block(mOwner->GetGame(), nullptr);
				Block* leftB = new Block(mOwner->GetGame(), nullptr);

				rightB->SetPosition(Vector3(x, y, 0));
				leftB->SetPosition(Vector3(x, -y, 0));

				rightB->SetScale(scale);
				leftB->SetScale(scale);

				rightB->GetComponent<MeshComponent>()->SetTextureIndex(texIndex);
				leftB->GetComponent<MeshComponent>()->SetTextureIndex(texIndex);

				y += scale;
			}
		}
		else
		{
			float y = scale;

			Block* middleB = new Block(mOwner->GetGame(), nullptr);
			middleB->SetPosition(Vector3(x, 0.0f, 0));
			middleB->SetScale(scale);
			middleB->GetComponent<MeshComponent>()->SetTextureIndex(texIndex);

			for(int j=0; j<(col-1)/2; j++)
			{
				Block* rightB = new Block(mOwner->GetGame(), nullptr);
				Block* leftB = new Block(mOwner->GetGame(), nullptr);

				rightB->SetPosition(Vector3(x, y, 0));
				leftB->SetPosition(Vector3(x, -y, 0));

				rightB->SetScale(scale);
				leftB->SetScale(scale);

				rightB->GetComponent<MeshComponent>()->SetTextureIndex(texIndex);
				leftB->GetComponent<MeshComponent>()->SetTextureIndex(texIndex);

				y += scale;
			}
        }
		x += scale;
	}
}


void PlayerMoveBuild::ClearAllBlocks()
{
	vector<Actor*> blocks = mOwner->GetGame()->Object;

	for(auto b: blocks)
	{
		b->SetState(ActorState::Destroy);
	}
}


void PlayerMoveBuild::GenerateSideBlock()
{
	float y = 750.0f;
	float x = 0.0f;
	for(int i=0; i<20; i++)
	{
		float z = 500.0f;
		for(int j=0; j<4; j++)
		{
			Block* l = new Block(mOwner->GetGame(), nullptr);
			Block* r = new Block(mOwner->GetGame(), nullptr);
			r->SetPosition(Vector3(x, y, z));
			l->SetPosition(Vector3(x, -y, z));

			r->SetScale(500.0f);
			l->SetScale(500.0f);

			r->GetComponent<MeshComponent>()->SetTextureIndex(4);
			l->GetComponent<MeshComponent>()->SetTextureIndex(4);

			z += 500.0f;
		}
		x += 500.0f;
	}
}


void PlayerMoveBuild::WriteBlockToFile()
{
	string name = "Assets/Map/Maze/zzDIY/" + mMapFileName + ".json";
	ofstream os(name);

	os<<"{"<<endl;
	os<<"\"actors\":"<<endl;
	os<<"["<<endl;

	if(mStartPoint)
	{
		os<<"\t{ \"type\":\"Player\", \"pos\":[";
			Vector3 pos = mStartPoint->GetPosition();
			os<<static_cast<int>(pos.x)<<".0, "
			  <<static_cast<int>(pos.y)<<".0, "
			  <<static_cast<int>(pos.z + 200.0f)<<".0]},"
			  <<endl;
	}

	vector<Actor*> object = mOwner->GetGame()->mActors;
   
	for(auto o: object)
	{
		if(o == mOwner) {continue;}

		if(o == mStartPoint) {continue;}

		Block* b = dynamic_cast<Block*>(o); 
		if(b) {continue;}
		
		Arrow* a = dynamic_cast<Arrow*>(o);
		if(a) {continue;}
		
		Checkpoint* cp = dynamic_cast<Checkpoint*>(o);
		if(cp)
		{
			os<<"\t{ \"type\":\"Checkpoint\", \"pos\":[";
			Vector3 pos = cp->GetPosition();
			os<<static_cast<int>(pos.x)<<".0, "<<static_cast<int>(pos.y)<<".0, "<<static_cast<int>(pos.z)<<".0]";

			if(cp->GetFallingDist() != 1500.0f)
			{
				os<<", \"falling\":"<<static_cast<int>(cp->GetFallingDist())<<".0";
			}

			if(!cp->GetText().empty())
			{
				os<<", \"text\":\""<<cp->GetText()<<"\"";
			}
			os<<" },"<<endl;
		}
		
		Coin* c = dynamic_cast<Coin*>(o);
		if(c)
		{
			os<<"\t{ \"type\":\"Coin\", \"pos\":[";
			Vector3 pos = c->GetPosition();
			os<<static_cast<int>(pos.x)<<".0, "<<static_cast<int>(pos.y)<<".0, "<<static_cast<int>(pos.z)<<".0]},"<<endl;
		}
		
		LaserMine* l = dynamic_cast<LaserMine*>(o);
		if(l)
		{
			os<<"\t{ \"type\":\"LaserMine\", \"pos\":[";

			if(l->GetMovable())
			{
				Vector3 pos = l->GetStartPos();
				os<<static_cast<int>(pos.x)<<".0, "<<static_cast<int>(pos.y)<<".0, "<<static_cast<int>(pos.z)<<".0]";
				os<<", \"mStartPoint\":["<<static_cast<int>(pos.x)<<".0, "<<static_cast<int>(pos.y)
					<<".0, "<<static_cast<int>(pos.z)<<".0]";
				Vector3 end = l->GetEndPos();	
				os<<", \"mEndPoint\":["<<static_cast<int>(end.x)<<".0, "<<static_cast<int>(end.y)
					<<".0, "<<static_cast<int>(end.z)<<".0], ";
				os<<"\"mMoveTime\":"<<l->GetMoveTime()<<".0";
			}
			else
			{
				Vector3 pos = l->GetPosition();
				os<<static_cast<int>(pos.x)<<".0, "<<static_cast<int>(pos.y)<<".0, "<<static_cast<int>(pos.z)<<".0]";
			}

			os<<", \"quat\":[";
			if(l->GetRotatable())
			{
				Quaternion quat = l->GetStartQuat();
                os<<quat.x; if(quat.x == 0.0f || quat.x == 1.0f){os<<".0";} os<<", ";
                os<<quat.y; if(quat.y == 0.0f || quat.x == 1.0f){os<<".0";} os<<", ";
                os<<quat.z; if(quat.z == 0.0f || quat.x == 1.0f){os<<".0";} os<<", ";
                os<<quat.w; if(quat.w == 0.0f || quat.x == 1.0f){os<<".0";} os<<"], ";
				os<<"\"mStartQuat\":[";
                os<<quat.x; if(quat.x == 0.0f || quat.x == 1.0f){os<<".0";} os<<", ";
                os<<quat.y; if(quat.y == 0.0f || quat.x == 1.0f){os<<".0";} os<<", ";
                os<<quat.z; if(quat.z == 0.0f || quat.x == 1.0f){os<<".0";} os<<", ";
                os<<quat.w; if(quat.w == 0.0f || quat.x == 1.0f){os<<".0";} os<<"], ";
				Quaternion endq = l->GetEndQuat();
				os<<"\"mEndQuat\":[";
                os<<endq.x; if(endq.x == 0.0f || quat.x == 1.0f){os<<".0";} os<<", ";
                os<<endq.y; if(endq.y == 0.0f || quat.x == 1.0f){os<<".0";} os<<", ";
                os<<endq.z; if(endq.z == 0.0f || quat.x == 1.0f){os<<".0";} os<<", ";
                os<<endq.w; if(endq.w == 0.0f || quat.x == 1.0f){os<<".0";} os<<"], ";
                os<<"\"mRotationTime\":"<<l->GetRotateTime()<<".0";
			}
			else
			{
				Quaternion quat = l->GetQuaternion();
                os<<quat.x; if(quat.x == 0.0f || quat.x == 1){os<<".0";} os<<", ";
                os<<quat.y; if(quat.y == 0.0f || quat.y == 1){os<<".0";} os<<", ";
                os<<quat.z; if(quat.z == 0.0f || quat.z == 1){os<<".0";} os<<", ";
                os<<quat.w; if(quat.w == 0.0f || quat.w == 1){os<<".0";} os<<"]";
			}

			if(l->GetLaserLength() != 500.0f)
			{
				os<<", \"length\":"<<static_cast<int>(l->GetLaserLength())<<".0";
			}

			if(l->GetLaserRandom())
			{
				os<<", \"random\":true";
			}

			os<<"},"<<endl;
		}

		SecurityCamera* s = dynamic_cast<SecurityCamera*>(o);
		if(s)
		{
			os<<"\t{ \"type\":\"SecurityCamera\", \"pos\":[";
			Vector3 pos = s->GetPosition();
			os<<static_cast<int>(pos.x)<<".0, "<<static_cast<int>(pos.y)<<".0, "<<static_cast<int>(pos.z)<<".0]";
			os<<", \"quat\":[";
			Quaternion quat = s->GetStartQ();
            os<<quat.x; if(quat.x == 0.0f){os<<".0";} os<<", ";
            os<<quat.y; if(quat.y == 0.0f){os<<".0";} os<<", ";
            os<<quat.z; if(quat.z == 0.0f){os<<".0";} os<<", ";
            os<<quat.w; if(quat.w == 0.0f){os<<".0";} os<<"], ";
			os<<"\"startQ\":[";
            os<<quat.x; if(quat.x == 0.0f){os<<".0";} os<<", ";
            os<<quat.y; if(quat.y == 0.0f){os<<".0";} os<<", ";
            os<<quat.z; if(quat.z == 0.0f){os<<".0";} os<<", ";
            os<<quat.w; if(quat.w == 0.0f){os<<".0";} os<<"], ";
			Quaternion endq = s->GetEndQ();
			os<<"\"endQ\":[";
            os<<endq.x; if(endq.x == 0.0f){os<<".0";} os<<", ";
            os<<endq.y; if(endq.y == 0.0f){os<<".0";} os<<", ";
            os<<endq.z; if(endq.z == 0.0f){os<<".0";} os<<", ";
            os<<endq.w; if(endq.w == 0.0f){os<<".0";} os<<"], ";
            os<<"\"interpTime\":"<<s->GetInterpTime()<<".0, ";
            os<<"\"pauseTime\":"<<s->GetPauseTime()<<".0},"<<endl;
		}
	}

	vector<Actor*> blocks = mOwner->GetGame()->Object;
	sort(blocks.begin(), blocks.end(), mScaleComparator);
	
	Block* lastB = nullptr;
	bool last = false;

	for(int i=0; i<blocks.size(); i++)
	{
		Block* b = static_cast<Block*> (blocks[i]);

		if(b->GetLast()) 
		{
			lastB = b;
			last = true;
			continue;
		}

		os<<"\t{ \"type\":\"Block\", \"pos\":[";
		if(b->GetMovable())
		{
			Vector3 pos = b->GetStartPos();
			os<<static_cast<int>(pos.x)<<".0, "<<static_cast<int>(pos.y)<<".0, "<<static_cast<int>(pos.z)<<".0]";
			os<<", \"mStartPoint\":["<<static_cast<int>(pos.x)<<".0, "<<static_cast<int>(pos.y)
				<<".0, "<<static_cast<int>(pos.z)<<".0]";
			Vector3 end = b->GetEndPos();	
			os<<", \"mEndPoint\":["<<static_cast<int>(end.x)<<".0, "<<static_cast<int>(end.y)
				<<".0, "<<static_cast<int>(end.z)<<".0]";
			os<<", \"mMoveTime\": "<<b->GetMoveTime()<<", ";
		}
		else
		{
			Vector3 pos = b->GetPosition();
			os<<static_cast<int>(pos.x)<<".0, "<<static_cast<int>(pos.y)<<".0, "<<static_cast<int>(pos.z)<<".0], ";
		}

		if(b->GetRotation() != 0.0f)
		{
			os<<"\"rot\":"<<b->GetRotation()<<", ";
		}

		if(b->GetBalance() != 0)
		{
			os<<"\"balance\":"<<b->GetBalance()<<", ";
		}

		if(b->GetStopMove())
		{
			os<<"\"stopmove\":"<<b->GetStopMove()<<", ";
		}

		os<<"\"scale\":"<<static_cast<int>(b->GetScale())<<".0, ";

		if(b->GetTurnOver() != 0) {os<<"\"turn\":"<<b->GetTurnOver()<<", ";}
		if(b->GetMirror()) {os<<"\"mirror\":true, ";}

		float index;
		if(b == mCurrentBlock)
		{
			 index = mOwner->GetGame()->player->GetComponent<HUDBuild>()->GetTargetIndex();
		}
		else 
		{
            index = b->GetComponent<MeshComponent>()->GetTextureIndex();
		}

		os<<"\"texture\":"<<index;
		
		os<<" }";

		if(i < blocks.size()-1) { os<<",";}
		if(i == blocks.size()-1 && last) { os<<",";}
		os<<endl;
	}


	if(lastB)
	{
		os<<"\t{ \"type\":\"Block\", \"pos\":[";
		Vector3 pos = lastB->GetPosition();
		os<<static_cast<int>(pos.x)<<".0, "<<static_cast<int>(pos.y)<<".0, "<<static_cast<int>(pos.z)<<".0], ";
		if(lastB->GetRotation() != 0.0f)
		{
			os<<"\"rot\":"<<lastB->GetRotation()<<", ";
		}
		os<<"\"scale\":"<<static_cast<int>(lastB->GetScale())<<".0, ";
		if(lastB->GetTurnOver() != 0) {os<<"\"turn\":"<<lastB->GetTurnOver()<<", ";}
		os<<"\"texture\":"<<lastB->GetComponent<MeshComponent>()->GetTextureIndex()<<", ";
		os<<"\"last\":true }";
	}

	os<<"],"
		<<"\"backgroundMusic\": \""<<"Empty"<<"\"}"<<endl;
	os.close();
	mOwner->GetGame()->mMazeMap.clear();
	mOwner->GetGame()->ListFileName("Assets/Map/Maze", 0, mOwner->GetGame()->mMazeMap);
}










