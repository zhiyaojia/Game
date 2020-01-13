#pragma once
#include "Actor.h"
#include <vector>

class Checkpoint: public Actor
{
public:
	Checkpoint(class Game* game, Actor* parent);

	void OnUpdate(float deltaTime) override;

	void Active();
	void Deactive();

	void SetFallingDist(float dist) {fallingDist = dist;}
	float GetFallingDist() {return fallingDist;}

    void SetText(std::string text) {this->text = text;}
	std::string GetText() {return text;}

	void SetLevel(std::string str){mLevelString = str;}

private:
	class MeshComponent* Checkpoint_mesh = nullptr;
	class CollisionComponent* Checkpoint_collision = nullptr;

	bool active = false;

	std::string mLevelString;
	std::string text = "";

	float fallingDist = 1500.0f;
};
