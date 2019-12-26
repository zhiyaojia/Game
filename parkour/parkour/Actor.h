#pragma once
#include <vector>
#include <SDL/SDL_stdinc.h>
#include "Math.h"
#include "Component.h"

enum class ActorState
{
	Active,
	Paused,
	Destroy
};

class Actor
{
public:
	Actor(class Game* game, Actor* parent);
	virtual ~Actor();

	// Update function called from Game (not overridable)
	void Update(float deltaTime);
	// ProcessInput function called from Game (not overridable)
	void ProcessInput(const Uint8* keyState);

	// Getters/setters
	const Vector3& GetPosition() const { return mPosition; }
	void SetPosition(const Vector3& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }

	Vector3 GetForward();
	Vector3 GetRight();

	bool GetMirror(){return mMirror;}
	void SetMirror(bool m){mMirror = m;}
	void SetBalance(bool balance){mBalance = balance;}
	bool GetBalance(){return mBalance;}

	void AddChild(Actor* child);
	void RemoveChild(Actor* child);

	Actor* GetParent(){return mParent;}

	void CalcWorldTransform();
	Matrix4 CalcWorldRotTrans();

	Vector3 GetWorldPosition();
	Vector3 GetWorldForward();

	Quaternion GetQuaternian(){return mQuaternion;}
	void SetQuaternian(Quaternion Quaternion){mQuaternion = Quaternion;}
	Vector3 GetQuatForward();
	
	ActorState GetState() const { return mState; }
	void SetState(ActorState state) { mState = state; }

	class Game* GetGame() { return mGame; }
	
	// Adds component to Actor (this is automatically called
	// in the component constructor)
	void AddComponent(Component* c);
	
	// Returns component of type T, or null if doesn't exist
	template <typename T>
	T* GetComponent() const
	{
		for (auto c : mComponents)
		{
			T* t = dynamic_cast<T*>(c);
			if (t != nullptr)
			{
				return t;
			}
		}
		
		return nullptr;
	}

	const Matrix4& GetWorldTransform() const {return mWorldTransform;}

protected:
	// Any actor-specific update code (overridable)
	virtual void OnUpdate(float deltaTime);
	// Any actor-specific update code (overridable)
	virtual void OnProcessInput(const Uint8* keyState);
	
	class Game* mGame;
	// Actor's state
	ActorState mState;

	// Transform
	Vector3 mPosition;
	float mScale;
	float mRotation;
	
	// Components0
	std::vector<Component*> mComponents;

	// Matrix
	Matrix4 mWorldTransform;

	// Quaternian
	Quaternion mQuaternion = Quaternion::Identity;

	// Block Properties
	bool mMirror = false;
	bool mBalance = false;

	// Inheritance
	Actor* mParent;
	std::vector<Actor*> mChildren;
	bool mInheritScale = false;
};










