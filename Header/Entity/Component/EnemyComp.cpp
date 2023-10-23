#include "EnemyComp.h"
#include "Rigidbody.h"
#include "../../../Engine/DirectBase/Model/ModelManager.h"
#include "Collider.h"
#include "SpringObjectComp.h"
#include "ModelComp.h"

std::list<std::unique_ptr<Entity>> *EnemyComp::sEnemys_ = {};
float EnemyComp::sStanTime_ = 0.f;

void EnemyComp::Init() {
	object_->AddComponent<Rigidbody>();

	currentState_ = std::make_unique<EnemyState::IdleState>(this);
	nextState_ = nullptr;

	auto *const enemyModel = ModelManager::GetInstance()->GetModel("Enemy");

	object_->AddComponent<ModelComp>()->AddBone("Body", enemyModel, Transform{ .translate{ 0.f,-1.f,0.f } });

	auto *const colliderComp = object_->AddComponent<ColliderComp>();
	colliderComp->SetRadius(1.f);
	colliderComp->SetCollisionAttribute(static_cast<uint32_t>(CollisionFilter::Enemy));
	colliderComp->SetCollisionMask(~static_cast<uint32_t>(CollisionFilter::Enemy));
}

void EnemyComp::Update([[maybe_unused]] float deltaTime) {

	if (nextState_ != nullptr) {
		currentState_->Exit(deltaTime);
		currentState_ = std::move(nextState_);
		currentState_->Init(deltaTime);
		nextState_ = nullptr;
	}
	currentState_->Update(deltaTime);

	if (sStanTime_ <= 0.f) {
		isStan_ = false;
	}

}

void EnemyComp::Draw([[maybe_unused]] const Camera<Render::CameraType::Projecction> &camera) const {

}

void EnemyComp::OnCollision([[maybe_unused]] Entity *const other) {

}

void EnemyComp::Destroy() {

}

void EnemyComp::BreakAll() {
	object_->SetActive(false);

	for (auto &enemy : *sEnemys_) {
		if (enemy->GetComponent<EnemyComp>()->GetIsStan()) {
			enemy->SetActive(false);
		}
	}
}

void EnemyComp::StaticUpdate(float deltaTime) {
	if (sStanTime_ > 0.f) {
		sStanTime_ -= deltaTime;
	}
}

void EnemyComp::StaticInit() {
	sStanTime_ = 0.f;
}

void EnemyComp::StartStan() {
	isStan_ = true;
	if (sStanTime_ <= 0.f) {
		sStanTime_ = vDefaultStanTime_;
	}
}

void EnemyState::IdleState::Init([[maybe_unused]] float deltaTime) {

}

void EnemyState::IdleState::Update([[maybe_unused]] float deltaTime) {
	if (enemy_->GetIsStan()) {
		enemy_->SetState<StunState>();
	}
}

void EnemyState::IdleState::Exit([[maybe_unused]] float deltaTime) {

}

void EnemyState::AttackState::Init([[maybe_unused]] float deltaTime) {

}

void EnemyState::AttackState::Update([[maybe_unused]] float deltaTime) {

}

void EnemyState::AttackState::Exit([[maybe_unused]] float deltaTime) {

}

void EnemyState::StunState::Init([[maybe_unused]] float deltaTime) {

}

void EnemyState::StunState::Update([[maybe_unused]] float deltaTime) {

	if (!enemy_->GetIsStan()) {
		enemy_->SetState<IdleState>();
	}
	enemy_->object_->transform_.rotate.y += 0.1f;

}

void EnemyState::StunState::Exit([[maybe_unused]] float deltaTime) {

}
