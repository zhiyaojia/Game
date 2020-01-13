#include "PlayerMove.h"
#include "Player.h"
#include "Game.h"
#include "Block.h"
#include "Renderer.h"
#include "Random.h"
#include "CameraComponent.h"
#include "Checkpoint.h"
#include "PauseMenu.h"
#include "HUDMaze.h"
#include "HUDTutorial.h"
#include "HUDPractice.h"
#include "QuitScreen.h"
#include <iostream>
#include <ctime>

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


PlayerMove::PlayerMove(Actor* owner, std::string g): MoveComponent(owner)
{
	ChangeState(PlayerMove::Falling);

	mRunningSFX = Mix_PlayChannel(mOwner->GetGame()->mRunningSound,
				 mOwner->GetGame()->GetSound("Assets/Sounds/Running.wav"), -1);
	Mix_Pause(mRunningSFX);

	input = PlayerMove::Blank;

	mCurrentMode = g;

	if(mCurrentMode == "Tutorial") {mLife = 999;}
}


PlayerMove::~PlayerMove()
{
	Mix_HaltChannel(mRunningSFX);
}


/* ==========================================  Process Input  =============================================*/

void PlayerMove::ProcessInput(const Uint8* keyState)
{
	if(finish) {return;}

	float speed = 0.0f;
	Vector3 forward = Vector3::Zero;
	if(mCurrentState == PlayerMove::Flying)
	{
		speed = Speed_Flying;
		forward = FlyingForward();
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
			if(mCurrentState == PlayerMove::Flying)
			{
				ChangeState(PlayerMove::Falling);
				mOwner->GetComponent<CameraComponent>()->SetAngleLimit(Math::PiOver2 / 2.0f);
			}
			else
			{
				ChangeState(PlayerMove::Flying);
				mOwner->GetComponent<CameraComponent>()->SetAngleLimit(Math::PiOver2 / 1.1f);
			}
		}
	}
	else
	{
		this->mKPressed = false;
	}

	

	if(mCurrentState == PlayerMove::Balance)
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
			input = PlayerMove::Blank;
		}

		if(keyState[SDL_SCANCODE_W])
		{
			mWPressed = true;

			if(mCtrlIsPressing)
			{
				if(input == PlayerMove::W && mCurrentState == PlayerMove::OnGround && !DisableSquat)
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
			else if(keyState[SDL_SCANCODE_LSHIFT] && !DisableFastRun)
			{
				if(input == PlayerMove::W && mCurrentState == PlayerMove::OnGround)
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
				input = PlayerMove::W;
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

		if(mCurrentState != PlayerMove::Flying)
		{
			if(keyState[SDL_SCANCODE_SPACE])
			{
				if(!mSpacePressed && mCurrentState == PlayerMove::OnGround &&
					      !HeightChange && input != PlayerMove::Shift) 
				{
					Mix_PlayChannel(mOwner->GetGame()->mJumpSound,
							 mOwner->GetGame()->GetSound("Assets/Sounds/Jump.wav"), 0);
					mSpacePressed = true;
					AddForce(mJumpForce);
					ChangeState(PlayerMove::Jump);

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
					if(input == PlayerMove::Ctrl)
					{
						AddForce(forward * speed);
					}
				}
				else
				{
					input = PlayerMove::Ctrl;

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

			if(input == PlayerMove::Blank)
			{
				input = PlayerMove::Shift;
			}
			
			if(keyState[SDL_SCANCODE_SPACE])
			{
				if(!mSpacePressed && mCurrentState == PlayerMove::OnGround)
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

		if(keyState[SDL_SCANCODE_P])
		{
			if(!this->mPPressed)
			{
				this->mPPressed = true;
				Vector3 pos = mOwner->GetPosition();
				pos.Print();
				cout<<endl;
			}
		}
		else
		{
			this->mPPressed = false;
		}

		float x_rotation = (static_cast<float>(x)/500.0f) * Math::Pi * 10.0f;
		SetAngularSpeed(x_rotation);
	}
}


void PlayerMove::HandleKeyPress(int key)
{
	if(key == SDLK_ESCAPE)
	{
		new PauseMenu(mOwner->GetGame());
	}
}


/* ==========================================  Update Functions  =============================================*/

void PlayerMove::Update(float deltaTime)
{
	if(finish) 
	{
		mFinishTimer -= deltaTime;
		if(mFinishTimer <= 0.0f)
		{
			new QuitScreen(mOwner->GetGame());
		}
		return;
	}

	touchBottom = false;

	if(mCurrentState == PlayerMove::OnGround)
	{
		UpdateOnGround(deltaTime);
	}
	else if(mCurrentState == PlayerMove::Jump)
	{
		UpdateJump(deltaTime);
	}
	else if(mCurrentState == PlayerMove::Falling)
	{
		UpdateFalling(deltaTime);
	}
	else if(mCurrentState == PlayerMove::WallClimb)
	{
		UpdateWallClimb(deltaTime);
	}
	else if(mCurrentState == PlayerMove::WallRun)
	{
		UpdateWallRun(deltaTime);
	}
	else if(mCurrentState == PlayerMove::Flying)
	{
		UpdateFlying(deltaTime);
	}
	else if(mCurrentState == PlayerMove::Sliding)
	{
		UpdateSliding(deltaTime);
	}
	else if(mCurrentState == PlayerMove::Balance)
	{
		UpdateBalance(deltaTime);
	}

// Check Sound
	if(
	   ((mCurrentState == PlayerMove::OnGround && mVelocity.Length() >= 50.0f) && !mSquat)||
		mCurrentState == PlayerMove::WallClimb ||
		mCurrentState == PlayerMove::WallRun)
	{
		Mix_Resume(mRunningSFX);
	}
	else
	{
		Mix_Pause(mRunningSFX);
	}

// Respawn
	if(Dead())
	{
		Respawn();
	}

// CheckPoints
	if(!mOwner->GetGame()->CheckpointArray.empty())
	{
		Checkpoint* checkpoint = mOwner->GetGame()->CheckpointArray.front();
		CollisionComponent* checkpoint_collision = checkpoint->GetComponent<CollisionComponent>();
		CollisionComponent* player_collision = mOwner->GetGame()->player->GetComponent<CollisionComponent>();
		
		if(player_collision->Intersect(checkpoint_collision))
		{	
			Vector3 respawnPos = checkpoint->GetPosition();
			respawnPos.z += 250.0f;
			mOwner->GetGame()->player->SetRespawn(respawnPos);
			mCurrentRotation = mOwner->GetRotation();
			if(mCurrentMode == "Maze")
			{
				checkpoint->Deactive();
				mOwner->GetGame()->CheckpointArray.pop();
				if(!mOwner->GetGame()->CheckpointArray.empty())
				{
					checkpoint = mOwner->GetGame()->CheckpointArray.front();
					checkpoint->Active();
				}

				mOwner->GetGame()->player->GetComponent<HUDMaze>()->SetShowText(1);	
			}
			else if(mCurrentMode == "Tutorial")
			{
				mOwner->GetGame()->player->GetComponent<HUDTutorial>()->
						updateCheckpointText(checkpoint->GetText());
				
				Mix_PlayChannel(mOwner->GetGame()->mCheckPointSound, 
							mOwner->GetGame()->GetSound("Assets/Sounds/Checkpoint.wav"), 0);
				checkpoint->Deactive();
				mOwner->GetGame()->CheckpointArray.pop();
				if(!mOwner->GetGame()->CheckpointArray.empty())
				{
					checkpoint = mOwner->GetGame()->CheckpointArray.front();
					checkpoint->Active();
				}
			}

			if(mOwner->GetGame()->CheckpointArray.empty()) 
			{
				Finish();
			}
		}
	}

// Test
	UpdatePrint(deltaTime);
}


void PlayerMove::UpdatePrint(float deltaTime)
{
	
}


void PlayerMove::PhysicsUpdate(float deltaTime)
{
	mAcceleration = mPendingForces * (1.0f / mMass);

	mVelocity += mAcceleration * deltaTime;

	if(mCurrentState == PlayerMove::Flying)
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


void PlayerMove::UpdateSliding(float deltaTime)
{
	PhysicsUpdate(deltaTime);

	mSlideTimer += deltaTime;

	if(mSlideTimer > 0.4f)
	{
		ChangeState(PlayerMove::OnGround);
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


void PlayerMove::UpdateFlying(float deltaTime)
{
	PhysicsUpdate(deltaTime);
}


void PlayerMove::UpdateOnGround(float deltaTime)
{
	PhysicsUpdate(deltaTime);

	bool falling = true;
	
	Vector3 pos = mOwner->GetPosition();
	vector<Actor*> blocks = mOwner->GetGame()->Object;
	CollisionComponent* playerCollision = mOwner->GetComponent<CollisionComponent>();
	for(auto b: blocks)
	{
		Block* block = static_cast<Block*>(b);
		
		Vector3 dist = pos - block->GetPosition();
		if(dist.Length() > 1000) {continue;}
	
		CollisionComponent* blockCollision = block->GetComponent<CollisionComponent>();
		CollSide side = FixCollision(playerCollision, blockCollision);

		if(block->GetStopMove()) 
		{
			mVelocity = Vector3::Zero;
		}

		if(side == CollSide::Top)
		{
			int balance = block->GetBalance();
			if(balance != 0)
			{
				SetUpBalance(balance);
				ChangeState(PlayerMove::Balance);
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
		ChangeState(PlayerMove::Falling);
	}

	UpdateSquating(deltaTime);
	UpdateFixJump(deltaTime);
	UpdateFastRun(deltaTime);
}


void PlayerMove::UpdateJump(float deltaTime)
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
		ChangeState(PlayerMove::Falling);
	}
}


void PlayerMove::UpdateFalling(float deltaTime)
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
				ChangeState(PlayerMove::Sliding);
			}
			else
			{
				ChangeState(PlayerMove::OnGround);
			}

			if(mFastJump)
			{
				mFastJump = false;
			}
		}
	}
}


void PlayerMove::UpdateWallClimb(float deltaTime)
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


void PlayerMove::UpdateWallRun(float deltaTime)
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


void PlayerMove::UpdateBalance(float deltaTime)
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
		ChangeState(PlayerMove::Falling);
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

void PlayerMove::ChangeState(MoveState state)
{
	mCurrentState = state;
}


bool PlayerMove::CanWallRun(CollSide side)
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


bool PlayerMove::CanWallClimb(CollSide side)
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


CollSide PlayerMove::FixCollision(class CollisionComponent* self, class CollisionComponent* block)
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


void PlayerMove::AddForce(const Vector3& force)
{
	mPendingForces += force;
}


void PlayerMove::FixXYVelocity()
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
	else if(mCurrentState == PlayerMove::Balance && mUnderBalance)
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

	if(mCurrentState == PlayerMove::OnGround || 
	   mCurrentState == PlayerMove::WallClimb || 
	   mCurrentState == PlayerMove::Balance)
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


void PlayerMove::FixXYZVelocity()
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


void PlayerMove::FixJump()
{
	if(mFixJumpTimer < 0.0f)
	{
		mFixJumpTimer = 0.0f;
	}
	float XYjumpForce = 30000.0f + Math::Lerp(0, 110000.0f, mFixJumpTimer/MaxJumpTime);
	float ZjumpForce = Math::Lerp(25000, 40000.0f, mFixJumpTimer/MaxJumpTime);
	Vector3 jumpForward = Vector3(mOwner->GetForward().x*XYjumpForce, mOwner->GetForward().y*XYjumpForce, ZjumpForce);
	AddForce(jumpForward);
	ChangeState(PlayerMove::Jump);
}


Vector3 PlayerMove::FlyingForward()
{
	Vector3 forward = mOwner->GetForward();
	forward.z = -Math::Sin(mOwner->GetComponent<CameraComponent>()->GetPitchAngle());
	return forward;
}


void PlayerMove::UpdateSquating(float deltaTime)
{
	if(!touchBottom && !(mCurrentState == PlayerMove::Sliding))
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


void PlayerMove::UpdateFixJump(float deltaTime)
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


void PlayerMove::UpdateFastRun(float deltaTime)
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


void PlayerMove::SetUpBalance(int balance)
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


void PlayerMove::FixBalanceAngle(float deltaTime)
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


void PlayerMove::Respawn()
{
	mOwner->SetPosition(mOwner->GetGame()->player->GetRespawn());
	mOwner->SetRotation(mCurrentRotation);
	mVelocity = Vector3::Zero;
	mPendingForces = Vector3::Zero;
	ChangeState(Falling);
	mOwner->GetComponent<CameraComponent>()->SetUp(0.0f, 0.0f);
	
	mCameraCaught = false;
	mCameraDetect = false;
	mLaserHit = false;
}


bool PlayerMove::Dead()
{
	if(mOwner->GetPosition().z < -750.0f)
	{
		mLife--;
		//cout<<"falling z:"<<mOwner->GetPosition().z<<"Fallingdist: "<<mFallingDist<<endl;
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


void PlayerMove::Finish() 
{
	finish = true;
	Mix_Pause(mRunningSFX);
}













