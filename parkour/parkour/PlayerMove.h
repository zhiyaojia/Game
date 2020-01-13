#pragma once
#include "Math.h"
#include <string>
#include <vector>
#include "MoveComponent.h"
#include "CollisionComponent.h"

class PlayerMove: public MoveComponent
{
public:
	enum MoveState
	{
		OnGround,
		Jump,
		Falling,
		WallClimb,
		WallRun,
		Flying,
		Sliding,
		Balance
	};

	enum Input
	{
		Blank,
		Shift,
		W,
		Ctrl
	};

	PlayerMove(class Actor* owner, std::string g);
	~PlayerMove();

	void Update(float deltaTime) override;

	void ProcessInput(const Uint8* keyState) override;
	void HandleKeyPress(int key) override;

	void ChangeState(MoveState state);

	void PhysicsUpdate(float deltaTime);

	void AddForce(const Vector3& force);

	void FixXYVelocity();
	void FixXYZVelocity();

	bool Dead();
	void Respawn();

    void Finish();
    
	void GetCaught() {mCameraCaught = true;}
	void GetDetect() {mCameraDetect = true;}
	void GetLaserHit() {mLaserHit = true;}

	int GetLife() {return mLife;}
	void AddLife() {mLife++;}

private:
	MoveState mCurrentState;
	Input input;

	Vector3 mVelocity;
	Vector3 mAcceleration;
	Vector3 mPendingForces;

	const float mMass = 1.0f;
	float height = 250.0f;

	float mWallClimbTimer = 0.0f;
	float mWallRunTimer = 0.0f;
	float mFixJumpTimer = -0.3f;
	float HeightChangeTimer = 0.0f;
	float mSlideTimer = 0.0f;
	float mFastRunTimer = 0.0f;

	Vector2 mBalanceIndex = Vector2::Zero;
	const float mBalanceRate = 0.3f; // inbalance speed
	const float mBalanceControlRate = 0.01f; // speed to balance for player
	float mIdealBalanceAngle = 0.0f;
	const float mBalanceAngleRate = 50.0f; // speed to switch to ideal angle
	int mBalanceAngleMultiplier = 0;
	Vector2 mBalanceDirection = Vector2::Zero;
	const float MaxBalanceAngle = 0.35f; // max inbalance angle

	const Vector3 mGravity = Vector3(0.0f, 0.0f, -980.0f);
	const Vector3 SuperJumpForce = Vector3(0.0f, 0.0f, 70000.0f);
	const Vector3 NormalJumpForce = Vector3(0.0f, 0.0f, 35000.0f);
	Vector3 mJumpForce = NormalJumpForce;
	const Vector3 mClimbForce = Vector3(0.0f, 0.0f, 2000.0f);
	const Vector3 mWallRunForce = Vector3(0.0f, 0.0f, 1200.0f);

	bool mSpacePressed = false;
	bool mWPressed = false;
	bool mKPressed = false;
	bool mPPressed = false;
	bool mJPressed = false;
	bool mCtrlPressed = false;
	bool mCtrlIsPressing = false;
	bool mShiftPressed = false;
	bool mShiftIsPressing = false;

	bool superjump = false;

	bool mFastRun = false;
	bool DisableFastRun = false;
	bool mFastJump = false;

	bool mSquat = false;
	bool HeightChange = false;
	bool HeightChangeStop = false;
	bool touchBottom = false;
	bool squatLand = false;

	bool mSliding = false;
	bool SlidingOnce = false;
	bool DisableSquat = false;

	bool mUnderBalance = false;

protected:
	void UpdateOnGround(float deltaTime);
	void UpdateJump(float deltaTime);
	void UpdateFalling(float deltaTime);
	void UpdateWallClimb(float deltaTime);
	void UpdateWallRun(float deltaTime);
	void UpdateFlying(float deltaTime);
	void UpdateSliding(float deltaTime);
	void UpdatePrint(float deltaTime);
	void UpdateBalance(float deltaTime);

	void UpdateSquating(float deltaTime);
	void UpdateFixJump(float deltaTime);
	void UpdateFastRun(float deltaTime);

	void SetUpBalance(int balance);
	void FixBalanceAngle(float deltaTime);

	void FixJump();

	bool CanWallClimb(CollSide side);
	bool CanWallRun(CollSide side);

	Vector3 FlyingForward();

	CollSide FixCollision(class CollisionComponent* self, class CollisionComponent* block);

	Vector2 mWallRunMultiplier = Vector2::Zero;

	int mRunningSFX;

	std::string mCurrentMode;

	int mLife;

	float mCurrentRotation = 0.0f;

	bool mCameraCaught = false;
	bool mCameraDetect = false;
	bool mLaserHit = false;

	bool finish = false;
	float mFinishTimer = 3.0f;
};







