#include "SpringObjectComp.h"
#include "ModelComp.h"
#include "../../../Engine/DirectBase/Model/ModelManager.h"
#include "Rigidbody.h"

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

}

void SpringObjectComp::Update(float deltaTime) {

	state_->Update(deltaTime);

	auto *const rigidbody = object_->GetComponent<Rigidbody>();

	// 毎フレームかかる処理はdeltaTimeをかける
	rigidbody->ApplyContinuousForce(Vector3::up * -9.8f, deltaTime);

}

void DefaultState::Update(float deltaTime) {
	if (Input::GetInstance()->GetDirectInput()->IsTrigger(DIK_SPACE)) {
		stateManager_->ChangeState<JumpingState>();
	}
	deltaTime;
}

void FallingState::Update(float deltaTime) {
	if (stateManager_->parent_->object_->GetComponent<Rigidbody>()->GetIsGround()) {
		stateManager_->ChangeState<DefaultState>();
	}
	deltaTime;
}

void JumpingState::Init(float deltaTime) {
	auto *const rigidbody = stateManager_->parent_->object_->GetComponent<Rigidbody>();
	rigidbody->ApplyInstantForce(Vector3::up * stateManager_->parent_->vJumpString_);
	deltaTime;
}

void JumpingState::Update(float deltaTime) {
	if (stateManager_->parent_->object_->GetComponent<Rigidbody>()->GetVelocity().y < 0.f) {
		stateManager_->ChangeState<FallingState>();
	}
	deltaTime;
}
