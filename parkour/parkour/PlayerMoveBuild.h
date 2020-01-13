#pragma once
#include "Math.h"
#include <string>
#include <vector>
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "BlockInfoStack.h"

class PlayerMoveBuild: public MoveComponent
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

	PlayerMoveBuild(class Actor* owner);
	~PlayerMoveBuild();

	void Update(float deltaTime) override;

	void ProcessInput(const Uint8* keyState) override;
	void HandleKeyPress(int key) override;

	void ChangeState(MoveState state);

	void PhysicsUpdate(float deltaTime);

	void AddForce(const Vector3& force);

	void ChangeTexture(int y);

	void FixXYVelocity();
	void FixXYZVelocity();

	bool IsFlying() {return mCurrentState == Flying;}

	void Respawn();

	Vector3 GetLastLandPos(){return mLastLandPos;}

	void GenerateBlocks(int row, int col, float scale, int texIndex);
	void ClearAllBlocks();

	void WriteBlockToFile();

	std::string GetFileName() {return mMapFileName;}

	class Checkpoint* mStartPoint;
	class Checkpoint* mFinalPoint;

private:
	void GenerateSideBlock();

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

	void UpdateBlock(float deltaTime);
	void UpdateObject(float deltaTime);
	
	void FixJump();

	bool CanWallClimb(CollSide side);
	bool CanWallRun(CollSide side);

	Vector3 FlyingForward();

	CollSide FixCollision(class CollisionComponent* self, class CollisionComponent* block);

	Vector2 mWallRunMultiplier = Vector2::Zero;

	int mRunningSFX;

	Vector3 mLastLandPos;

	std::vector<std::string> mTextureNames;

	class Actor* mCurrentBlock;

	BlockInfoStack mLastBlockStack;

	Vector3 mBlockVelocity;
	float mBlockScaleSpeed;

	int mDefaultBlockTexIndex = 5;
	float mDefaultBlockScale = 500.0f;
	float mDefaultBlockRotation = 0.0f;

	bool mPreciseTarget = false;
	bool CreateLaser = false;
	bool CreateCamera = false;
	bool CreateMirrorBlock = false;
	bool CreateCoin = false;
	bool CreateCheckPoint = false;
	bool CreateStartPoint = false;
	bool CreateFinalPoint = false;

	Vector3 ObjectMoveVelocity = Vector3::Zero;
	Quaternion ObjectQuatVelocity = Quaternion(0.0f, 0.0f, 0.0f, 0.0f);

	class Actor* mObject = nullptr;

	std::string mMapFileName;

	float mStartRotation, mEndRotation;
	float mTurnOverTime = 0.8f;
	float mTurnOverTimer = -0.1f;
	bool mOnTurning = false;
	bool mOnTurnOverBlock = false;
};









