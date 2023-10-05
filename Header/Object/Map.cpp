#include "Map.h"

void Map::Init() {
	pizza_.reset(Sprite::Create(TextureManager::Load("Ball.png")));
	pizza_->SetScale({ 540.f,540.f });
	pizza_->SetPivot({ 0.5f, 0.5f });
	pizza_->SetPosition(Vector2{ 1280.f, 720.f } / 2.f);
	pizza_->SetColor({ 1.f,0.75f,1.f, 1.f });

}

void Map::Update(const float deltaTime) {
	deltaTime;
}

void Map::Draw() {
	pizza_->Draw();
}
