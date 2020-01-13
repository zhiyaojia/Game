#include "PlayerMoveAdventure.h"
#include "Player.h"
#include "Game.h"
#include "Renderer.h"
#include "Random.h"
#include "CameraComponent.h"
#include "Checkpoint.h"
#include "Block.h"
#include <iostream>
#include <ctime>
#include "HUDAdventure.h"
#include "AdventurePauseScreen.h"
#include "AdventureRespawnScreen.h"

using namespace std;

const float MaxSpeed_Balance = 100.0f;
const float Speed_Balance = 100.0f;

const float MaxJumpTime = 2.0f;

const float mSlideForce = 200.0f;
const float MaxSpeed_Sliding = 2000.0f;


PlayerMoveAdventure::PlayerMoveAdventure(Actor* owner): MoveComponent(owner)
{
	ChangeState(PlayerMoveAdventure::Falling);

	mRunningSFX = Mix_PlayChannel(mOwner->GetGame()->mRunningSound, 
		mOwner->GetGame()->GetSound("Assets/Sounds/Running.wav"), -1);
	Mix_Pause(mRunningSFX);

	input = PlayerMoveAdventure::Blank;

	mResume = false;
}


PlayerMoveAdventure::~PlayerMoveAdventure()
{
	Mix_HaltChannel(mRunningSFX);
}


/* ==========================================  Process Input  =============================================*/

void PlayerMoveAdventure::ProcessInput(const Uint8* keyState)
{	
	if(mCameraDetect || !mResume || mLose) {return;}

	Vector3 forward = mOwner->GetForward();

	if(mCurrentState == PlayerMoveAdventure::Balance)
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
	}
	else
	{
		if(keyState[SDL_SCANCODE_LCTRL])
		{
			if(mCurrentState == PlayerMoveAdventure::OnGround && !DisableSquat)
			{
				mOwner->GetComponent<CollisionComponent>()->SetSize(50.0f, 125.0f, 50.0f);

				mSliding = true;
				squatLand = true;
				DisableSquat = true;

				AddForce(mGravity);

				Mix_PlayChannel(mOwner->GetGame()->mSlideSound, 
					mOwner->GetGame()->GetSound("Assets/Sounds/slide.ogg"), 0);
			}
		}
		else if(keyState[SDL_SCANCODE_LSHIFT] && !DisableFastRun && !mStopMove)
		{
			if(input == PlayerMoveAdventure::W && mCurrentState == PlayerMoveAdventure::OnGround)
			{
				if(!mShiftPressed)
				{
					mFastRunTimer = 0.0f;
					mShiftPressed = true;
				}

				mFastRun = true;

				AddForce(forward * Speed_Standing * 3);
			}
		}
		else
		{
			mFastRun = false;
			if(!mStopMove)
			{
				input = PlayerMoveAdventure::W;
				AddForce(forward * Speed_Standing);
			}
			if(!keyState[SDL_SCANCODE_LSHIFT])
			{
				DisableFastRun = false;
			}
		}

		if(keyState[SDL_SCANCODE_A])
		{
			AddForce(mOwner->GetRight() * -Speed_Standing * 2);
		}
		else if(keyState[SDL_SCANCODE_D])
		{
			AddForce(mOwner->GetRight() * Speed_Standing * 2);
		}
		else if(keyState[SDL_SCANCODE_A] && keyState[SDL_SCANCODE_D])
		{
			AddForce(Vector3::Zero);
		}
		else if(!keyState[SDL_SCANCODE_A] && !keyState[SDL_SCANCODE_D])
		{
			AddForce(Vector3::Zero);
		}

		if(keyState[SDL_SCANCODE_SPACE])
		{
			if(!mSpacePressed && mCurrentState == PlayerMoveAdventure::OnGround &&
				      !HeightChange && input != PlayerMoveAdventure::Shift && !mStopMove) 
			{
				Mix_PlayChannel(mOwner->GetGame()->mJumpSound, 
					mOwner->GetGame()->GetSound("Assets/Sounds/Jump.wav"), 0);
				mSpacePressed = true;
				AddForce(mJumpForce);
				ChangeState(PlayerMoveAdventure::Jump);

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

		if(keyState[SDL_SCANCODE_LSHIFT] && mStopMove)
		{
			mShiftIsPressing = true;
			if(keyState[SDL_SCANCODE_SPACE] && mCurrentState == PlayerMoveAdventure::OnGround)
			{
				if(!mSpacePressed)
				{
					mSpacePressed = true;
					FixJump();
					mFixJumpTimer = -0.1;
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
	}
}


void PlayerMoveAdventure::HandleKeyPress(int key)
{
	if(key == SDLK_ESCAPE)
	{
		new AdventurePauseScreen(mOwner->GetGame());
	}
}


/* ==========================================  Update Functions  =============================================*/

void PlayerMoveAdventure::Update(float deltaTime)
{
	if(mLife <= 0) {mLose = true;}
	if(mLose)
	{
		mLoseTimer -= deltaTime;
		if(mLoseTimer < 0.0f) 
		{
			int gamescore = mOwner->GetGame()->GetAdventureScore();
			if(mScore > gamescore)
			{
				mOwner->GetGame()->SetAdventureScore(mScore);
				breakRecord = true;
			}
			new AdventureRespawnScreen(mOwner->GetGame());
		}
		return;
	}

	if(!mResume) 
	{ 
		Mix_Pause(mRunningSFX); 
		return;
	}

	touchBottom = false;

	if(mCurrentState == PlayerMoveAdventure::OnGround)
	{
		UpdateOnGround(deltaTime);
	}
	else if(mCurrentState == PlayerMoveAdventure::Jump)
	{
		UpdateJump(deltaTime);
	}
	else if(mCurrentState == PlayerMoveAdventure::Falling)
	{
		UpdateFalling(deltaTime);
	}
	else if(mCurrentState == PlayerMoveAdventure::WallClimb)
	{
		UpdateWallClimb(deltaTime);
	}
	else if(mCurrentState == PlayerMoveAdventure::WallRun)
	{
		UpdateWallRun(deltaTime);
	}
	else if(mCurrentState == PlayerMoveAdventure::Sliding)
	{
		UpdateSliding(deltaTime);
	}
	else if(mCurrentState == PlayerMoveAdventure::Balance)
	{
		UpdateBalance(deltaTime);
	}

// Check Sound
	if((
	   ((mCurrentState == PlayerMoveAdventure::OnGround && mVelocity.Length() >= 50.0f) && !mSquat)||
		mCurrentState == PlayerMoveAdventure::WallClimb ||
		mCurrentState == PlayerMoveAdventure::WallRun) && !mLose)
	{
		Mix_Resume(mRunningSFX);
	}
	else
	{
		Mix_Pause(mRunningSFX);
	}

// CheckPoints
	if(!mOwner->GetGame()->CheckpointArray.empty())
	{
		Checkpoint* checkpoint = mOwner->GetGame()->CheckpointArray.front();

		if((mOwner->GetPosition().x > checkpoint->GetPosition().x && mDirection == 0) ||
			(mOwner->GetPosition().y < checkpoint->GetPosition().y && mDirection == -1) ||
			(mOwner->GetPosition().y > checkpoint->GetPosition().y && mDirection == 1))
		{
			mOwner->GetGame()->player->SetRespawn(checkpoint->GetPosition());
			mCurrentRotation = mOwner->GetRotation();
			mOwner->GetGame()->CheckpointArray.pop();
			mFallingDist = checkpoint->GetPosition().z - checkpoint->GetFallingDist();
            checkpoint->SetState(ActorState::Destroy);
		}
	}

// Respawn
	if(((mDirection == 0 && mVelocity.x <= 25.0f) || 
	   (mDirection != 0 && abs(mVelocity.y) <= 25.0f)) && !mStopMove && !mOnTurning)
	{
		mDeadTimer+=deltaTime;
	}
	else
	{
		mDeadTimer = 0.0f;
	}

	if(Dead())
	{
		Respawn();
	}

// Create Actors
	float dist = (mOwner->GetGame()->GetFinalPos() - mOwner->GetPosition()).Length();
	if(dist < 5000.0f)
	{
		mOwner->GetGame()->LoadAdvanture();
	}

// Delete Actors
	vector<Actor*> actor = mOwner->GetGame()->mActors;
	for(auto a: actor)
	{
		if(a == mOwner) continue;

		float dist = mOwner->GetPosition().x - a->GetPosition().x;
		if(dist > 5000.0f)
		{
			a->SetState(ActorState::Destroy);
		}
	}

// Update TurnOver
	if(mOnTurning)
	{
		mDeadTimer = 0.0f;
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

// Update standSpeed 
	mSpeedTimer -= deltaTime;
	if(mSpeedTimer <= 0.0f)
	{
		Speed_Standing *= speedMultiplier;
		MaxSpeed_Standing *= speedMultiplier;
		mSpeedTimer = 20.0f;
	}

// Update Score
	if(!mCameraDetect)
    {
    	mScore += 2.0f * deltaTime;
    }

// Test
	UpdatePrint(deltaTime);
}


void PlayerMoveAdventure::UpdatePrint(float deltaTime)
{
	
}


void PlayerMoveAdventure::PhysicsUpdate(float deltaTime)
{
	mAcceleration = mPendingForces * (1.0f / mMass);

	mVelocity += mAcceleration * deltaTime;

	FixXYVelocity();

	Vector3 pos = mOwner->GetPosition();
	pos += mVelocity * deltaTime;
	mOwner->SetPosition(pos);

	mOwner->SetRotation(mOwner->GetRotation() + deltaTime * GetAngularSpeed());

	mPendingForces = Vector3::Zero;
}


void PlayerMoveAdventure::UpdateSliding(float deltaTime)
{
	PhysicsUpdate(deltaTime);

	mSlideTimer += deltaTime;

	if(mSlideTimer > 0.4f)
	{
		ChangeState(PlayerMoveAdventure::OnGround);
		mSlideTimer = 0.0f;
		HeightChange = true;
		mSliding = false;
		DisableSquat = false;
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


void PlayerMoveAdventure::UpdateOnGround(float deltaTime)
{	
	PhysicsUpdate(deltaTime);

	bool falling = true;
	bool touchTurnOverBlock = false;
	bool stopmove = false;

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
				ChangeState(Balance);
			}
			else
			{
				mLastLandPos = block->GetPosition();
				mLastLandPos.x -= (block->GetScale())*0.5f;
				mLastLandPos.z += (block->GetScale())*0.5f;
			}

			int TurnOverDir = block->GetTurnOver();

			if(TurnOverDir != 0 && mDirection != TurnOverDir)
			{
				touchTurnOverBlock = true;
				if(!mOnTurning && !mOnTurnOverBlock)
				{
					mOnTurning = true;
					mOnTurnOverBlock = true;
					mStartRotation = mOwner->GetRotation();
					mEndRotation = mStartRotation + TurnOverDir * Math::PiOver2;
					mDirection += TurnOverDir;
				}
			}

			if(block->GetStopMove()) 
			{
				stopmove = true;
				mVelocity = Vector3::Zero;
			}

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
		ChangeState(Falling);
	}
	if(!touchTurnOverBlock)
	{
		mOnTurnOverBlock = false;
	}
	
	mStopMove = stopmove;

	UpdateSquating(deltaTime);
	UpdateFixJump(deltaTime);
	UpdateFastRun(deltaTime);
}


void PlayerMoveAdventure::UpdateJump(float deltaTime)
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
		ChangeState(PlayerMoveAdventure::Falling);
	}
}


void PlayerMoveAdventure::UpdateFalling(float deltaTime)
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
				Mix_PlayChannel(mOwner->GetGame()->mLandSound, 
					mOwner->GetGame()->GetSound("Assets/Sounds/Land.wav"), 0);
			}
			else
			{
				squatLand = false;
			}

			if(mSliding)
			{
				ChangeState(PlayerMoveAdventure::Sliding);
			}
			else
			{
				ChangeState(PlayerMoveAdventure::OnGround);
			}

			if(mFastJump)
			{
				mFastJump = false;
			}
		}
		else if(CollisionComponent::IsSide(side))
		{
			if(CanWallClimb(side))
			{
				ChangeState(WallClimb);
				mWallClimbTimer = 0.0f;
				return;
			}
		}
	}
}


void PlayerMoveAdventure::UpdateWallClimb(float deltaTime)
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


void PlayerMoveAdventure::UpdateWallRun(float deltaTime)
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


void PlayerMoveAdventure::UpdateBalance(float deltaTime)
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
		ChangeState(PlayerMoveAdventure::Falling);
		camera->FixUp();
		camera->SetRotationSpeed(0.8f);
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

void PlayerMoveAdventure::ChangeState(MoveState state)
{
	mCurrentState = state;
}


bool PlayerMoveAdventure::CanWallRun(CollSide side)
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
	if(dot1 >= 0.5f || dot1 <= -0.3)
	{
		return false;
	}

	Vector3 VelocityDirection = mVelocity;
	VelocityDirection.Normalize();
	float dot2 = Vector3::Dot(VelocityDirection, mOwner->GetForward());
	dot2 -= 1;
	if(dot2 >= 0.3f || dot2 <= -0.3f)
	{
		return false;
	}

	if(mVelocity.Length() <= 150.0f)
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


bool PlayerMoveAdventure::CanWallClimb(CollSide side)
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

	return true;
}


CollSide PlayerMoveAdventure::FixCollision(class CollisionComponent* self, class CollisionComponent* block)
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
            if(abs(Vector3::Dot(mOwner->GetForward(), Vector3::UnitX)) == 1)
			{
				AddForce(Vector3(-Speed_Standing, 0.0f, 0.0f));
			}
			else
			{
				AddForce(Vector3(-Speed_Standing*2, 0.0f, 0.0f));
			}
		}
		else if(side == CollSide::SideX2)
		{
            if(abs(Vector3::Dot(mOwner->GetForward(), Vector3::UnitX)) == 1)
			{
				AddForce(Vector3(Speed_Standing, 0.0f, 0.0f));
			}
			else
			{
				AddForce(Vector3(Speed_Standing*2, 0.0f, 0.0f));
			}
		}
		else if(side == CollSide::SideY1)
		{
            if(abs(Vector3::Dot(mOwner->GetForward(), Vector3::UnitY)) == 1)
			{
				AddForce(Vector3(0.0f, -Speed_Standing, 0.0f));
			}
			else
			{
				AddForce(Vector3(0.0f, -Speed_Standing*2, 0.0f));
			}
		}
		else if(side == CollSide::SideY2)
		{
            if(abs(Vector3::Dot(mOwner->GetForward(), Vector3::UnitY)) == 1)
			{
				AddForce(Vector3(0.0f, Speed_Standing, 0.0f));
			}
			else
			{
				AddForce(Vector3(0.0f, Speed_Standing*2, 0.0f));
			}
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


void PlayerMoveAdventure::AddForce(const Vector3& force)
{
	mPendingForces += force;
}


void PlayerMoveAdventure::FixXYVelocity()
{
	Vector2 xyVelocity = Vector2(mVelocity.x, mVelocity.y);
	
	float MaxSpeed = 0.0f;

	if(mCurrentState == Sliding)
	{
		MaxSpeed = MaxSpeed_Sliding;
	}
	else if(mCurrentState == Falling || mCurrentState == Jump || mCurrentState == WallRun)
	{
		MaxSpeed = mFastJump? 400.0 * 3: 400.0f;
	}
	else if(mCurrentState == PlayerMoveAdventure::Balance && mUnderBalance)
	{
		MaxSpeed = MaxSpeed_Balance;
	}
	else if(mFastRun)
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

	if(mCurrentState == PlayerMoveAdventure::OnGround || 
	   mCurrentState == PlayerMoveAdventure::WallClimb || 
	   mCurrentState == PlayerMoveAdventure::Balance)
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


void PlayerMoveAdventure::FixJump()
{
	if(mFixJumpTimer < 0.0f)
	{
		mFixJumpTimer = 0.0f;
	}
	
	float XYjumpForce = 30000.0f + Math::Lerp(0, 110000.0f, mFixJumpTimer/MaxJumpTime);
	float ZjumpForce = Math::Lerp(25000, 40000.0f, mFixJumpTimer/MaxJumpTime);
	Vector3 jumpForward = Vector3(mOwner->GetForward().x*XYjumpForce, 
		mOwner->GetForward().y*XYjumpForce, ZjumpForce);
	AddForce(jumpForward);
	ChangeState(PlayerMoveAdventure::Jump);
}


void PlayerMoveAdventure::UpdateSquating(float deltaTime)
{
	if(!touchBottom && !(mCurrentState == PlayerMoveAdventure::Sliding))
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


void PlayerMoveAdventure::UpdateFixJump(float deltaTime)
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


void PlayerMoveAdventure::UpdateFastRun(float deltaTime)
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


void PlayerMoveAdventure::SetUpBalance(int balance)
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


void PlayerMoveAdventure::Respawn()
{
	mOwner->SetPosition(mOwner->GetGame()->player->GetRespawn());
	mOwner->SetRotation(mCurrentRotation);
	mVelocity = Vector3::Zero;
	mPendingForces = Vector3::Zero;
	Speed_Standing = 700.0f;
	MaxSpeed_Standing = 400.0f;
	ChangeState(PlayerMoveAdventure::Falling);
	mOwner->GetComponent<CameraComponent>()->SetUp(0.0f, 0.0f);
	
	mCameraCaught = false;
	mCameraDetect = false;
	mLaserHit = false;
	mDeadTimer = 0.0f;
}


bool PlayerMoveAdventure::Dead()
{
	if(mOwner->GetPosition().z < mFallingDist)
	{
		mLife--;
		//cout<<"falling z:"<<mOwner->GetPosition().z<<"Fallingdist: "<<mFallingDist<<endl;
		return true;
	}	

	if(mDeadTimer > 1.5f)
	{
		//cout<<"mDeadTimer"<<endl;
		mLife--;
		return true;
	}

	if(mCameraCaught || mLaserHit)
	{
		//cout<<"c l"<<endl;
		mLife--;
		return true;
	}

	return false;
}
















