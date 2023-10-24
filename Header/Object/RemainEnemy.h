#pragma once
#include "../../Engine/DirectBase/2D/Sprite.h"
#include <memory>

class RemainEnemy {
public:
	void Init();
	void Update();
	void Draw();
	void SetNumber(int num) { number_ = num; }
	void SetMaxNumber(int num) { maxNumber_ = num; }

	void SetPosition(Vector2 position);
private:
	Vector2 pos_;
	
	std::unique_ptr<Sprite> numberSprite_[4];

	std::unique_ptr<Sprite> slashSprite_;

	float numWidth;
	float numHeight;

	int num_[4];
	int number_;
	int maxNumber_;

	float numScale = 0.5f;
};

