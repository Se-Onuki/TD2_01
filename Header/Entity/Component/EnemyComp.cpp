#include "EnemyComp.h"
#include "Rigidbody.h"
#include "../../../Engine/DirectBase/Model/ModelManager.h"
#include "Collider.h"
#include "SpringObjectComp.h"

std::list<std::unique_ptr<Entity>> *EnemyComp::sEnemys_ = {};
float EnemyComp::sStanTime_ = 0.f;

void EnemyComp::Init() {
	object_->AddComponent<Rigidbody>();

	currentState_ = std::make_unique<EnemyState::IdleState>(this);
	nextState_ = nullptr;

	auto *const enemyModel = ModelManager::GetInstance()->GetModel("Enemy");
	defaultModel_ = enemyModel;

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
					sStanTime_ = vDefaultStanTime_;
				}

				auto *const selfRigidbody = object_->GetComponent<Rigidbody>();
				selfRigidbody->SetVelocity(Vector3::zero);
				selfRigidbody->ApplyInstantForce(Vector3::up * -0.5f);
			}
			// 下から叩かれた場合
			else {
				// 自分自身がスタンしてる場合
				if (isStan_) {
					// スタン中の敵全破壊
					BreakAll();
				}
			}

		}
		// もし高速落下中ならば
		else if (dynamic_cast<const FallingState *const>(playerState)) {
			// スタン中の敵全破壊
			BreakAll();
		}
	}
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

void EnemyState::IdleState::Init([[maybe_unused]] float deltaTime) {

}

void EnemyState::IdleState::Update([[maybe_unused]] float deltaTime) {

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

	if (remainingTime_ <= 0.f) {

	}
}

void EnemyState::StunState::Exit([[maybe_unused]] float deltaTime) {

}
