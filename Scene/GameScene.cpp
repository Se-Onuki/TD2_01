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
	uint32_t backTexHandle = TextureManager::Load("backTexture.png");
	sprite_.reset(Sprite::Create());
	sprite_->SetTextureHaundle(backTexHandle);
	sprite_->SetScale({ 1280.0f, 720.0f });
	sprite_->SetPosition({ 0.0f, 0.0f });

	auto *const modelManager = ModelManager::GetInstance();
	modelManager->AddModel("Spring", Model::LoadObjFile("Model/Player/", "player.obj"));
	modelManager->AddModel("Enemy", Model::LoadObjFile("Model/Enemy/", "enemy.obj"));
	auto* const orb = modelManager->AddModel("Orb", Model::LoadObjFile("Model/Orb/", "orb.obj"));
	orb->materialMap_["Material"]->blendMode_ = Model::BlendMode::kNormal;
	orb->materialMap_["Material"]->materialBuff_->color = { 1.0f,1.0f, 1.0f, 0.6f };

	auto *const unbleakble = modelManager->AddModel("Unbreakable", Model::LoadObjFile("Model/Block/", "block.obj"));
	unbleakble->materialMap_["Material"]->texHandle_ = TextureManager::Load("Model/Block/unbreakableBlock_test.png");

	auto *const crack = modelManager->AddModel("Crack", Model::LoadObjFile("Model/Block/", "block.obj"));
	crack->materialMap_["Material"]->texHandle_ = TextureManager::Load("Model/Block/creakedBlock_test.png");

	auto *const box = modelManager->AddModel("Box", Model::LoadObjFile("Model/Block/", "block.obj"));
	box->materialMap_["Material"]->texHandle_ = TextureManager::Load("Model/Block/stoneBlock_test.png");

	modelManager->AddModel("Soul", Model::LoadObjFile("Model/Souls/purple_soul/", "purple_soul.obj"));

}

GameScene::~GameScene() {

}

void GameScene::OnEnter() {

	gameManager_ = GameManager::GetInstance();
	gameManager_->Init();

}

void GameScene::OnExit() {
	gameManager_->Exit();
}

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

		sceneManager_->ChangeScene<TitleScene>(60);
	}
}

void GameScene::Draw() {
	DirectXCommon *const dxCommon = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList *const commandList = dxCommon->GetCommandList();

#pragma region 背面スプライト

	Sprite::StartDraw(commandList);

	// スプライトの描画
	sprite_->Draw();
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
	gameManager_->Draw2D();

	Sprite::EndDraw();

#pragma endregion

}