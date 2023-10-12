#include "Gamemanager.h"
#include "../../Engine/DirectBase/Model/ModelManager.h"

#include "../Entity/Component/ModelComp.h"
#include "../Entity/Component/SpringObjectComp.h"
#include "../Entity/Component/Rigidbody.h"

void GameManager::Init() {

	//collisionManager_ = CollisionManager::GetInstance();

#pragma region Spring

	//spring_ = std::make_unique<Entity>();
	spring_.Init();
	spring_.AddComponent<SpringObjectComp>();
	spring_.timeScale_ = 2.f;
	spring_.GetComponent<Rigidbody>()->hasCollider_ = true;
	spring_.transform_.translate = Vector3{ 0.f,5.f,0.f };

#pragma endregion

#pragma region MapChip

	mapChip_ = MapChip::GetInstance();
	mapChip_->Init();

#pragma endregion

}

void GameManager::Update(const float deltaTime) {

	/*collisionManager_->clear();

	enemys_.remove_if([](std::unique_ptr<Entity> &enemy) {
		if (!enemy->GetActive()) {
			enemy.reset();
			return true;
		}
		return false;
		}
	);
	if (!spring_->GetActive()) {
		spring_.reset();
	}

	collisionManager_->push_back(spring_.get());
	for (auto &enemy : enemys_) {
		collisionManager_->push_back(enemy.get());
	}
	collisionManager_->ChackAllCollision();*/

	//if (spring_) {
		spring_.Update(deltaTime);
	//}
}

void GameManager::Draw(const Camera<Render::CameraType::Projecction> &camera) const {
	//if (spring_) {
		spring_.Draw(camera);
	//}
	mapChip_->Draw(camera);
}

void GameManager::ImGuiWidget() {
	//if (spring_) {
		spring_.ImGuiWidget();
	//}
}