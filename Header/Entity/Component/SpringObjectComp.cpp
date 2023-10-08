#include "SpringObjectComp.h"
#include "ModelComp.h"
#include "../../../Engine/DirectBase/Model/ModelManager.h"
#include "Rigidbody.h"

SpringObjectComp::~SpringObjectComp() {
}

void SpringObjectComp::Init() {
	input_ = Input::GetInstance();

	auto *const springModel = ModelManager::GetInstance()->GetModel("Spring");
	auto *const modelComp = object_->AddComponent<ModelComp>();
	modelComp->AddBone("Body", springModel);

	object_->AddComponent<Rigidbody>();

}

void SpringObjectComp::Update(float deltaTime) {
	if (input_->GetDirectInput()->IsTrigger(DIK_SPACE)) {
		Jump();
	}

	auto *const rigidbody = object_->GetComponent<Rigidbody>();
	rigidbody->AddAcceleration(Vector3::up * -9.8f * deltaTime);
	deltaTime;
}

void SpringObjectComp::Jump() {
	auto *const rigidbody = object_->GetComponent<Rigidbody>();
	rigidbody->AddAcceleration(Vector3::up * 10.f);
}
