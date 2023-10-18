#include "EnemyComp.h"
#include "Rigidbody.h"
#include "../../../Engine/DirectBase/Model/ModelManager.h"
#include "Collider.h"
#include "SpringObjectComp.h"

std::list<std::unique_ptr<Entity>> *EnemyComp::sEnemys_ = {};
float EnemyComp::sStanTime_ = 0.f;

void EnemyComp::Init() {
	object_->AddComponent<Rigidbody>();

	auto *const enemyModel = ModelManager::GetInstance()->GetModel("Enemy");
	defaultModel_ = enemyModel;

	auto *const colliderComp = object_->AddComponent<ColliderComp>();
	colliderComp->SetRadius(1.f);
	colliderComp->SetCollisionAttribute(static_cast<uint32_t>(CollisionFilter::Enemy));
	colliderComp->SetCollisionMask(~static_cast<uint32_t>(CollisionFilter::Enemy));
}

void EnemyComp::Update([[maybe_unused]] float deltaTime) {
	if (sStanTime_ <= 0.f) {
		isStan_ = false;
	}

}

void EnemyComp::Draw(const Camera<Render::CameraType::Projecction> &camera) const {
	defaultModel_->Draw(object_->transform_, camera);
}

void EnemyComp::OnCollision(Entity *const other) {
	// プレイヤのコンポーネントを取得
	auto *const springObjectComp = other->GetComponent<SpringObjectComp>();
	// もし存在したら実行
	if (springObjectComp) {
		auto *const playerState = springObjectComp->GetManager()->GetState();
		// もしジャンプ中ならば
		if (dynamic_cast<const JumpingState *const>(playerState)) {

			auto *const playerRigidbody = other->GetComponent<Rigidbody>();
			// 上から踏まれた場合
			if (playerRigidbody->GetVelocity().y <= 0.f) {

				isStan_ = true;
				if (sStanTime_ <= 0.f) {
					sStanTime_ = 10.f;
				}

				auto *const selfRigidbody = object_->GetComponent<Rigidbody>();
				selfRigidbody->SetVelocity(Vector3::zero);
				selfRigidbody->ApplyInstantForce(Vector3::up * -0.5f);
			}
			// 下から叩かれた場合
			else {
				if (isStan_) {
					// スタン中の敵全破壊
					SelfBreak();
				}
			}

		}
		// もし高速落下中ならば
		else if (dynamic_cast<const FallingState *const>(playerState)) {
			// スタン中の敵全破壊
			SelfBreak();
		}
	}
}

void EnemyComp::Destroy() {
}

void EnemyComp::SelfBreak() {
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
