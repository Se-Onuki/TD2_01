#include "GameScene.h"

#include <imgui.h>
#include "../Engine/DirectBase/Base/DirectXCommon.h"
#include "../Engine/DirectBase/Model/ModelManager.h"

#include "TitleScene.h"

#include "../Header/Entity/Component/ModelComp.h"
#include "../Utils/SoLib/SoLib_ImGui.h"

GameScene::GameScene() {
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ライトの生成
	light_.reset(DirectionLight::Create());

	camera_.translation_ = Vector3{ 0.f, 10.f, -30.f };
	camera_.UpdateMatrix();

	auto *const modelManager = ModelManager::GetInstance();
	modelManager->AddModel("Spring", Model::LoadObjFile("", "sphere.obj"));
	modelManager->AddModel("Enemy", Model::LoadObjFile("", "sphere.obj"));
	modelManager->AddModel("Gauge", Model::LoadObjFile("", "sphere.obj"));

	auto *const unbleakble = modelManager->AddModel("Unbreakable", Model::LoadObjFile("", "box.obj"));
	unbleakble->materialMap_["Material"]->materialBuff_->color = { 0.5f,0.5f,0.5f,1.f };

	auto *const crack = modelManager->AddModel("Crack", Model::LoadObjFile("", "box.obj"));
	crack->materialMap_["Material"]->materialBuff_->color = { 0.f,0.f,0.f,1.f };

	modelManager->AddModel("Box", Model::LoadObjFile("", "box.obj"));
	modelManager->AddModel("Soul", Model::LoadObjFile("", "sphere.obj"));

}

GameScene::~GameScene() {

}

void GameScene::OnEnter() {

	gameManager_ = GameManager::GetInstance();
	gameManager_->Init();

}

void GameScene::OnExit() {}

void GameScene::Update() {
	const float deltaTime = ImGui::GetIO().DeltaTime;

	ImGui::Begin("Camera");
	camera_.ImGuiWidget();
	ImGui::End();
	camera_.UpdateMatrix();

	ImGui::Begin("Sphere");
	//	model_->ImGuiWidget();
	//	transform_.ImGuiWidget();
	ImGui::End();

	TextureManager::GetInstance()->ImGuiWindow();
	gameManager_->Update(deltaTime);

	gameManager_->ImGuiWidget();

	light_->ImGuiWidget();
	if (input_->GetXInput()->IsTrigger(KeyCode::RIGHT_SHOULDER) ||
		input_->GetDirectInput()->IsTrigger(DIK_3)) {

		sceneManager_->ChangeScene(new TitleScene, 60);
	}
	//	transform_.UpdateMatrix();
}

void GameScene::Draw() {
	DirectXCommon *const dxCommon = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList *const commandList = dxCommon->GetCommandList();

#pragma region 背面スプライト

	Sprite::StartDraw(commandList);

	// スプライトの描画

	Sprite::EndDraw();

#pragma endregion


	dxCommon->CrearDepthBuffer();

#pragma region モデル描画

	Model::StartDraw(commandList);

	light_->SetLight(commandList);

	// モデルの描画
	gameManager_->Draw();

	Model::EndDraw();

#pragma endregion

#pragma region 前面スプライト

	Sprite::StartDraw(commandList);

	// スプライトの描画

	Sprite::EndDraw();

#pragma endregion

}