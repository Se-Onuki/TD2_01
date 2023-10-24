#include "RemainEnemy.h"

void RemainEnemy::Init() {
	numberSprite_[0].reset(Sprite::Create(TextureManager::Load("nums.png")));
	numberSprite_[1].reset(Sprite::Create(TextureManager::Load("nums.png")));
	
	slashSprite_.reset(Sprite::Create(TextureManager::Load("nums.png")));

	numberSprite_[2].reset(Sprite::Create(TextureManager::Load("nums.png")));
	numberSprite_[3].reset(Sprite::Create(TextureManager::Load("nums.png")));

	numWidth = numberSprite_[0]->GetUV().second.x / 10.0f;
	numHeight = numberSprite_[0]->GetUV().second.y;


	numberSprite_[0]->SetScale({numWidth * numScale, numHeight * numScale });
	numberSprite_[0]->SetPivot({0.5f, 0.5f});
	numberSprite_[1]->SetScale({numWidth * numScale, numHeight * numScale });
	numberSprite_[1]->SetPivot({0.5f, 0.5f});

	slashSprite_->SetScale({ numWidth * numScale, numHeight * numScale });
	slashSprite_->SetPivot({ 0.5f, 0.5f });

	numberSprite_[2]->SetScale({numWidth * numScale, numHeight * numScale });
	numberSprite_[2]->SetPivot({0.5f, 0.5f});
	numberSprite_[3]->SetScale({numWidth * numScale, numHeight * numScale });
	numberSprite_[3]->SetPivot({0.5f, 0.5f});

}

void RemainEnemy::Update() {
	num_[1] = number_ / 10;
	number_ = number_ % 10;
	num_[0] = number_ / 1;
	num_[2] = maxNumber_ / 10;
	number_ = maxNumber_ % 10;
	num_[3] = maxNumber_ / 1;

	numberSprite_[0]->SetPosition({pos_.x - 30.0f, pos_.y});
	numberSprite_[0]->SetTexOrigin({numWidth * num_[0], 0.0f});
	numberSprite_[0]->SetTexDiff({numWidth , numHeight});
	
	numberSprite_[1]->SetPosition({ numberSprite_[0]->GetTransform().translate.x - (numWidth * numScale), pos_.y});
	numberSprite_[1]->SetTexOrigin({numWidth * num_[1], 0.0f});
	numberSprite_[1]->SetTexDiff({numWidth , numHeight});

	slashSprite_->SetPosition({ pos_.x, pos_.y });
	slashSprite_->SetTexOrigin({ 0.0f, 0.0f });
	slashSprite_->SetTexDiff({ numWidth , numHeight });

	numberSprite_[2]->SetPosition({ pos_.x + 30.0f, pos_.y });
	numberSprite_[2]->SetTexOrigin({ numWidth * num_[2], 0.0f });
	numberSprite_[2]->SetTexDiff({ numWidth , numHeight });
	
	numberSprite_[3]->SetPosition({ numberSprite_[2]->GetTransform().translate.x + (numWidth * numScale), pos_.y });
	numberSprite_[3]->SetTexOrigin({ numWidth * num_[3], 0.0f });
	numberSprite_[3]->SetTexDiff({ numWidth , numHeight });

}

void RemainEnemy::Draw() {
	numberSprite_[0]->Draw();
	numberSprite_[1]->Draw();
	slashSprite_->Draw();
	numberSprite_[2]->Draw();
	numberSprite_[3]->Draw();
}

void RemainEnemy::SetPosition(Vector2 position) {
	pos_ = position;
}
