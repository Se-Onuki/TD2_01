#include "EnemyComp.h"
#include "Rigidbody.h"
#include "../../../Engine/DirectBase/Model/ModelManager.h"
#include "Collider.h"

void EnemyComp::Init() {
	object_->AddComponent<Rigidbody>();

	auto *const enemyModel = ModelManager::GetInstance()->GetModel("Enemy");
	defaultModel_ = enemyModel;

	auto *const colliderComp = object_->AddComponent<ColliderComp>();
	colliderComp->SetRadius(1.f);
	colliderComp->SetCollisionAttribute(static_cast<uint32_t>(CollisionFilter::Enemy));
	colliderComp->SetCollisionMask(~static_cast<uint32_t>(CollisionFilter::Enemy));
}

void EnemyComp::Update(float deltaTime) {
	deltaTime;
}

void EnemyComp::Draw(const Camera<Render::CameraType::Projecction> &camera) const {
	defaultModel_->Draw(object_->transform_, camera);
}

void EnemyComp::Destroy() {
}
