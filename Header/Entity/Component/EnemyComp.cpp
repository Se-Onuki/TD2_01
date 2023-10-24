#include "EnemyComp.h"
#include "Rigidbody.h"
#include "../../../Engine/DirectBase/Model/ModelManager.h"
#include "Collider.h"
#include "SpringObjectComp.h"
#include "ModelComp.h"

#include "../../Object/GameManager.h"

std::list<std::unique_ptr<Entity>> *EnemyComp::sEnemys_ = {};
float EnemyComp::sStanTime_ = 0.f;

void EnemyComp::Init() {
	object_->AddComponent<Rigidbody>();

	currentState_ = std::make_unique<EnemyState::IdleState>(this);
	nextState_ = nullptr;

	auto *const enemyModel = ModelManager::GetInstance()->GetModel("Enemy");

	object_->AddComponent<ModelComp>()->AddBone("Body", enemyModel, Transform{ .translate{ 0.f,-2.f,0.f } });

	auto *const colliderComp = object_->AddComponent<ColliderComp>();
	colliderComp->SetRadius(1.f);
	colliderComp->SetCollisionAttribute(static_cast<uint32_t>(CollisionFilter::Enemy));
	colliderComp->SetCollisionMask(~static_cast<uint32_t>(CollisionFilter::Enemy));

	stunSprite_.reset(Sprite::Create(TextureManager::Load("oneStun_effect.png")));
	stunSprite_->SetScale({ 100.f,100.f });
	stunSprite_->SetPivot({ 0.5f,0.5f });
}

void EnemyComp::Update([[maybe_unused]] float deltaTime) {

	if (sStanTime_ <= 0.f) {
		isStan_ = false;
	}

	if (nextState_ != nullptr) {
		currentState_->Exit(deltaTime);
		currentState_ = std::move(nextState_);
		currentState_->Init(deltaTime);
		nextState_ = nullptr;
	}
	currentState_->Update(deltaTime);

	stunSprite_->SetVisible(isStan_);
	if (isStan_) {
		stunSprite_->SetPosition(GameManager::GetInstance()->GetCamera()->GetScreenPoint(object_->GetWorldPos()).ToVec2());
		stunSprite_->SetRotate(stunSprite_->GetTransform().rotate.z + Angle::Dig2Rad * 1.f);
	}

}

void EnemyComp::Draw([[maybe_unused]] const Camera<Render::CameraType::Projecction> &camera) const {

}

void EnemyComp::Draw2D() const {
	stunSprite_->Draw();
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
	if (enemy_->GetIsStan()) {
		enemy_->SetState<StunState>();
	}

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

void EnemyState::ApproachState::Init([[maybe_unused]] float deltaTime) {

}

void EnemyState::ApproachState::Update([[maybe_unused]] float deltaTime) {
	if (enemy_->GetIsStan()) {
		enemy_->SetState<StunState>();
	}
}

void EnemyState::ApproachState::Exit([[maybe_unused]] float deltaTime) {

}

void EnemyState::StapUpState::Init([[maybe_unused]] float deltaTime) {

}

void EnemyState::StapUpState::Update([[maybe_unused]] float deltaTime) {

}

void EnemyState::StapUpState::Exit([[maybe_unused]] float deltaTime) {

}
