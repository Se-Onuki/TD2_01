#include "Gamemanager.h"
#include "../../Engine/DirectBase/Model/ModelManager.h"

#include "../Entity/Component/ModelComp.h"
#include "../Entity/Component/SpringObjectComp.h"
#include "../Entity/Component/Rigidbody.h"
#include "../Entity/Component/EnemyComp.h"
#include "../Entity/Component/OrbComp.h"

void GameManager::Init() {

	collisionManager_ = CollisionManager::GetInstance();

#pragma region Spring

	spring_ = std::make_unique<Entity>();
	spring_->Init();
	spring_->AddComponent<SpringObjectComp>();
	spring_->timeScale_ = 2.5f;
	spring_->GetComponent<Rigidbody>()->hasCollider_ = true;
	spring_->transform_.translate = Vector3{ 0.f,5.f,0.f };

#pragma endregion

#pragma region MapChip

	mapChip_ = MapChip::GetInstance();
	mapChip_->Init();

#pragma endregion

#pragma region Enemy

	AddEnemy({ -4.f,5.f,0.f });
	AddEnemy({ 3.f,10.f,0.f });
	EnemyComp::SetEnemyList(&enemys_);

#pragma endregion

#pragma region Orb

	orb_ = std::make_unique<Entity>();
	orb_->AddComponent<OrbComp>();
	orb_->Init();

#pragma endregion

#pragma region Camera

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->SetSpring(spring_.get());
	followCamera_->Reset();

#pragma endregion


}

void GameManager::Update(const float deltaTime) {

	enemys_.remove_if([](std::unique_ptr<Entity> &enemy) {
		if (!enemy->GetActive()) {
			enemy->Destroy();
			enemy.reset();
			return true;
		}
		return false;
		}
	);
	if (!spring_->GetActive()) {
		spring_.reset();
	}

	collisionManager_->clear();
	collisionManager_->push_back(spring_.get());
	for (auto &enemy : enemys_) {
		collisionManager_->push_back(enemy.get());
	}
	collisionManager_->ChackAllCollision();

	EnemyComp::StaticUpdate(deltaTime);

	if (spring_) {
		spring_->Update(deltaTime);
	}

	for (auto &enemy : enemys_) {
		enemy->Update(deltaTime);
	}

	if (orb_) {
		orb_->Update(deltaTime);
	}

	if (followCamera_) {
		followCamera_->Update(deltaTime);
	}
}

void GameManager::Draw() const {
	const auto &camera = *followCamera_->GetCamera();

	if (spring_) {
		spring_->Draw(camera);
	}

	for (auto &enemy : enemys_) {
		enemy->Draw(camera);
	}

	if (orb_) {
		orb_->Draw(camera);
	}
	mapChip_->Draw(camera);
}

void GameManager::ImGuiWidget() {
	if (spring_) {
		spring_->ImGuiWidget();
	}
}

void GameManager::AddEnemy(const Vector3 &pos) {
	enemys_.push_back(std::make_unique<Entity>());
	auto &newEnemy = enemys_.back();

	newEnemy->Init();

	newEnemy->AddComponent<EnemyComp>();
	newEnemy->transform_.translate = pos;
	newEnemy->transform_.UpdateMatrix();
}
