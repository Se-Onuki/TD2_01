#include "Gamemanager.h"
#include "../../Engine/DirectBase/Model/ModelManager.h"

#include "../Entity/Component/ModelComp.h"
#include "../Entity/Component/SpringObjectComp.h"
#include "../Entity/Component/Rigidbody.h"

void GameManager::Init() {

#pragma region Spring

	spring_.AddComponent<SpringObjectComp>();
	spring_.timeScale_ = 2.f;
	spring_.GetComponent<Rigidbody>()->hasCollider_ = true;
	spring_.transform_.translate = Vector3{ 0.f,5.f,0.f };

#pragma endregion

#pragma region MapChip

	mapChip_ = MapChip::GetInstance();
	mapChip_->Init();

#pragma endregion


}

void GameManager::Update(const float deltaTime) {
	spring_.Update(deltaTime);
}

void GameManager::Draw(const Camera<Render::CameraType::Projecction> &camera) {
	spring_.Draw(camera);
	mapChip_->Draw(camera);
}

void GameManager::ImGuiWidget() {
	spring_.ImGuiWidget();
}