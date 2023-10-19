#include "Gamemanager.h"
#include "../../Engine/DirectBase/Model/ModelManager.h"

#include "../Entity/Component/ModelComp.h"
#include "../Entity/Component/SpringObjectComp.h"
#include "../Entity/Component/Rigidbody.h"
#include "../Entity/Component/EnemyComp.h"
#include "../Entity/Component/OrbComp.h"
#include "../Entity/Component/SoulComp.h"

void GameManager::Init() {

	collisionManager_ = CollisionManager::GetInstance();

#pragma region Spring

	AddPlayer();

#pragma endregion

#pragma region MapChip

	mapChip_ = MapChip::GetInstance();
	mapChip_->Init();

#pragma endregion

#pragma region Enemy

	AddEnemy({ 0.f,10.f,0.f });
	AddEnemy({ 10.f,10.f,0.f });
	AddEnemy({ -10.f,10.f,0.f });
	EnemyComp::SetEnemyList(&enemys_);

#pragma endregion

#pragma region Orb

	orb_ = std::make_unique<Entity>();
	orb_->AddComponent<OrbComp>();
	orb_->Init();

	SoulComp::SetOrbComp(orb_.get());

#pragma endregion

#pragma region Camera

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Reset();

#pragma endregion


}

void GameManager::Exit() {
	enemys_.clear();
	souls_.clear();
	spring_.reset();
	collisionManager_->clear();

}

void GameManager::Update(const float deltaTime) {

	enemys_.remove_if([this](std::unique_ptr<Entity> &enemy) {
		if (!enemy->GetActive()) {
			AddSoul(enemy->GetWorldPos());
			enemy->Destroy();
			enemy.reset();
			return true;
		}
		return false;
		}
	);
	souls_.remove_if([](std::unique_ptr<Entity> &soul) {
		if (!soul->GetActive()) {
			soul->Destroy();
			soul.reset();
			return true;
		}
		return false;
		}
	);

	if (spring_ && !spring_->GetActive()) {
		spring_.reset();
	}

	collisionManager_->clear();
	if (spring_) {
		collisionManager_->push_back(spring_.get());
	}
	for (auto &enemy : enemys_) {
		collisionManager_->push_back(enemy.get());
	}
	collisionManager_->ChackAllCollision();

	EnemyComp::StaticUpdate(deltaTime);

	if (Input::GetInstance()->GetDirectInput()->IsTrigger(DIK_P)) {
		if (spring_) {
			spring_->SetActive(false);
		}
		else {
			AddPlayer();
		}
	}

	if (spring_) {
		spring_->Update(deltaTime);
	}

	for (auto &enemy : enemys_) {
		enemy->Update(deltaTime);
	}
	for (auto &soul : souls_) {
		soul->Update(deltaTime);
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
	for (auto &soul : souls_) {
		soul->Draw(camera);
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

	newEnemy->transform_.translate = pos;
	newEnemy->transform_.UpdateMatrix();
	newEnemy->AddComponent<EnemyComp>();
}

void GameManager::AddSoul(const Vector3 &pos) {
	souls_.push_back(std::make_unique<Entity>());
	auto &newSoul = souls_.back();

	newSoul->Init();
	newSoul->transform_.translate = pos;
	newSoul->transform_.UpdateMatrix();
	newSoul->AddComponent<SoulComp>();

}

void GameManager::AddPlayer() {
	spring_ = std::make_unique<Entity>();
	spring_->Init();
	spring_->AddComponent<SpringObjectComp>();
	spring_->timeScale_ = 3.f;
	spring_->GetComponent<Rigidbody>()->hasCollider_ = true;
	spring_->transform_.translate = Vector3{ 0.f,10.f,0.f };
}
