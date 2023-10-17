#include "EnemyComp.h"
#include "Rigidbody.h"
#include "../../../Engine/DirectBase/Model/ModelManager.h"
#include "Collider.h"
#include "SpringObjectComp.h"

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
	if (stanTime_ > 0.f) {
		stanTime_ -= deltaTime;


	}

}

void EnemyComp::Draw(const Camera<Render::CameraType::Projecction> &camera) const {
	defaultModel_->Draw(object_->transform_, camera);
}

void EnemyComp::OnCollision(Entity *const other) {
	auto *const springObjectComp = other->GetComponent<SpringObjectComp>();
	if (springObjectComp) {
		auto *const playerState = springObjectComp->GetState()->GetState();
		if (dynamic_cast<const JumpingState *const>(playerState)) {

			auto *const playerRigidbody = other->GetComponent<Rigidbody>();

			if (playerRigidbody->GetVelocity().y <= 0.f) {

				auto *const selfRigidbody = object_->GetComponent<Rigidbody>();
				selfRigidbody->SetVelocity(Vector3::zero);
				selfRigidbody->ApplyInstantForce(Vector3::up * -0.5f);
			}

		}
	}
}

void EnemyComp::Destroy() {
}
