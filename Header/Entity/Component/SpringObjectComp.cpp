#include "SpringObjectComp.h"
#include "ModelComp.h"
#include "../../../Engine/DirectBase/Model/ModelManager.h"
#include "Rigidbody.h"

#include "../../../Utils/SoLib/SoLib_Lerp.h"

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

void FallingState::Init(float) {
	objectTransform_ = &stateManager_->parent_->object_->transform_;
	startAngle_ = objectTransform_->rotate;
	endAngle_ = objectTransform_->rotate + Vector3{ 0.f,0.f,Angle::Dig2Rad * 180.f };
}

void FallingState::Update(float deltaTime) {
	auto *const rigidbody = stateManager_->parent_->object_->GetComponent<Rigidbody>();
	if (Input::GetInstance()->GetDirectInput()->IsPress(DIK_A)) {
		rigidbody->ApplyContinuousForce(Vector3::right * stateManager_->parent_->vMoveString_ * -1.f, deltaTime);
	}
	if (Input::GetInstance()->GetDirectInput()->IsPress(DIK_D)) {
		rigidbody->ApplyContinuousForce(Vector3::right * stateManager_->parent_->vMoveString_, deltaTime);
	}
	if (stateManager_->parent_->object_->GetComponent<Rigidbody>()->GetIsGround()) {
		stateManager_->ChangeState<DefaultState>();
	}

	if (t_ <= 1.f) {
		t_ = std::clamp((t_ + deltaTime / vNeedTime_), 0.f, 1.f);
		objectTransform_->rotate = SoLib::Lerp(startAngle_, endAngle_, t_);

		objectTransform_->UpdateMatrix();

	}
	deltaTime;
}

void FallingState::Exit(float) {
	objectTransform_->rotate = endAngle_;
	objectTransform_->UpdateMatrix();
}

void JumpingState::Init(float deltaTime) {
	auto *const rigidbody = stateManager_->parent_->object_->GetComponent<Rigidbody>();
	rigidbody->ApplyInstantForce(Vector3::up * stateManager_->parent_->vJumpString_);
	deltaTime;
}

void JumpingState::Update(float deltaTime) {

	auto *const rigidbody = stateManager_->parent_->object_->GetComponent<Rigidbody>();
	if (Input::GetInstance()->GetDirectInput()->IsPress(DIK_A)) {
		rigidbody->ApplyContinuousForce(Vector3::right * stateManager_->parent_->vMoveString_ * -1.f, deltaTime);
	}
	if (Input::GetInstance()->GetDirectInput()->IsPress(DIK_D)) {
		rigidbody->ApplyContinuousForce(Vector3::right * stateManager_->parent_->vMoveString_, deltaTime);
	}

	if (stateManager_->parent_->object_->GetComponent<Rigidbody>()->GetVelocity().y < 0.f) {
		stateManager_->ChangeState<FallingState>();
	}
	deltaTime;
}
