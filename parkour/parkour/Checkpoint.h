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

    void setText(std::string text) {this->text = text;}
	std::string getText() {return text;}

	void SetLevel(std::string str){mLevelString = str;}

private:
	class MeshComponent* Checkpoint_mesh;
	class CollisionComponent* Checkpoint_collision;

	bool active = false;

	std::string mLevelString;
	std::string text = "";
};
