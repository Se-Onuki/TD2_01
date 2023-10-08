#include "Gamemanager.h"
#include "../../Engine/DirectBase/Model/ModelManager.h"

#include "../Entity/Component/ModelComp.h"
#include "../Entity/Component/SpringObjectComp.h"

void GameManager::Init() {

#pragma region Spring

	spring_.AddComponent<SpringObjectComp>();

#pragma endregion



}

void GameManager::Update(const float deltaTime) {
	spring_.Update(deltaTime);
}

void GameManager::Draw(const Camera<Render::CameraType::Projecction> &camera) {
	spring_.Draw(camera);
}
