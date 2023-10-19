#include "SpringObjectComp.h"
#include "ModelComp.h"
#include "../../../Engine/DirectBase/Model/ModelManager.h"
#include "Rigidbody.h"

#include "../../../Utils/SoLib/SoLib_Lerp.h"
#include "Collider.h"
#include "EnemyComp.h"

SpringObjectComp::~SpringObjectComp() {
}

void SpringObjectComp::Init() {
	input_ = Input::GetInstance();

	state_ = std::make_unique<PlayerStateManager>(this);
	state_->Init();

	auto *const springModel = ModelManager::GetInstance()->GetModel("Spring");
	auto *const modelComp = object_->AddComponent<ModelComp>();
	modelComp->AddBone("Body", springModel);

	object_->AddComponent<Rigidbody>();

	auto *const colliderComp = object_->AddComponent<ColliderComp>();
	colliderComp->SetRadius(1.f);
	colliderComp->SetCollisionAttribute(static_cast<uint32_t>(CollisionFilter::Player));
	colliderComp->SetCollisionMask(~static_cast<uint32_t>(CollisionFilter::Player));

}

void SpringObjectComp::Update([[maybe_unused]] float deltaTime) {

	state_->Update(deltaTime);

	auto *const rigidbody = object_->GetComponent<Rigidbody>();

	// 毎フレームかかる処理はdeltaTimeをかける
	rigidbody->ApplyContinuousForce(Vector3::up * -9.8f, deltaTime);

	Vector3 vec = rigidbody->GetVelocity();
	vec.x = std::clamp(vec.x, vMaxSpeed_->x * -1.f, vMaxSpeed_->x);
	rigidbody->SetVelocity(vec);

}

void SpringObjectComp::OnCollision(Entity *const other) {
	state_->OnCollision(other);
}

void DefaultState::Update([[maybe_unused]] float deltaTime) {
	if (Input::GetInstance()->GetDirectInput()->IsPress(DIK_SPACE)) {
		stateManager_->ChangeState<SquattingState>();
	}
}

void FallingState::Init([[maybe_unused]] float deltaTime) {
	auto *const rigidbody = stateManager_->parent_->object_->GetComponent<Rigidbody>();
	rigidbody->SetVelocity(Vector3::zero);
	rigidbody->SetAcceleration(Vector3::up * -10.f);
}

void FallingState::Update([[maybe_unused]] float deltaTime) {
	if (stateManager_->parent_->object_->GetComponent<Rigidbody>()->GetIsGround()) {
		stateManager_->ChangeState<DefaultState>();
	}

}

void FallingState::Exit([[maybe_unused]] float deltaTime) {
}

void FallingState::OnCollision([[maybe_unused]] Entity *const other) {

}

void JumpingState::Init([[maybe_unused]] float deltaTime) {
	auto *const rigidbody = stateManager_->parent_->object_->GetComponent<Rigidbody>();
	rigidbody->SetVelocity(Vector3::zero);
	rigidbody->ApplyInstantForce(Vector3::up * stateManager_->parent_->vJumpString_);
}

void JumpingState::Update([[maybe_unused]] float deltaTime) {

	auto *const rigidbody = stateManager_->parent_->object_->GetComponent<Rigidbody>();
	if (Input::GetInstance()->GetDirectInput()->IsPress(DIK_A)) {
		rigidbody->ApplyContinuousForce(Vector3::right * stateManager_->parent_->vMoveString_ * -1.f, deltaTime);
	}
	if (Input::GetInstance()->GetDirectInput()->IsPress(DIK_D)) {
		rigidbody->ApplyContinuousForce(Vector3::right * stateManager_->parent_->vMoveString_, deltaTime);
	}

	if (Input::GetInstance()->GetDirectInput()->IsTrigger(DIK_SPACE)) {
		stateManager_->ChangeState<FallingState>();
	}

}

void JumpingState::OnCollision([[maybe_unused]] Entity *const other) {

	auto *const rigidbody = stateManager_->parent_->object_->GetComponent<Rigidbody>();
	if (rigidbody->GetVelocity().y <= 0.f) {
		stateManager_->ChangeState<JumpingState>();
	}
	else {
		auto *const enemyComp = other->GetComponent<EnemyComp>();
		if (enemyComp) {
			other->SetActive(false);
		}
	}
}

void SquattingState::Update([[maybe_unused]] float deltaTime) {
	if (Input::GetInstance()->GetDirectInput()->IsRelease(DIK_SPACE)) {
		stateManager_->ChangeState<JumpingState>();
	}
}