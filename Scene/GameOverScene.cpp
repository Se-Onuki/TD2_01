#include "GameOverScene.h"
#include "TitleScene.h"

#include <imgui.h>
#include "../Engine/DirectBase/Base/DirectXCommon.h"
#include "../Engine/DirectBase/Model/ModelManager.h"
//#include "../Header/Entity/Component/SkyCylinderComp.h"

bool GameOverScene::isChangeSceneCall_ = false;

GameOverScene::GameOverScene() {
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
}

GameOverScene::~GameOverScene() {
}

void GameOverScene::OnEnter() {
	light_.reset(DirectionLight::Create());
	camera_.translation_ = Vector3{ 0.f, 10.f, -30.f };
	camera_.UpdateMatrix();
	isChangeSceneCall_ = false;

	titleLogo_ = std::make_unique<TitleLogo>();
	titleLogo_->Init("GameOverLogo.png");
	pressSprite_ = std::make_unique<PressSprite>();
	pressSprite_->Init();

#pragma region SkyCylinderComp
	skyCylinder_ = std::make_unique<SkyCylinder>();
	//skyCylinder_ = std::make_unique<SkyCylinderComp>();
	//skyCylinder_->AddComponent<SkyCylinderComp>();
	skyCylinder_->Init("skyCylinder");

#pragma endregion

	Fade::GetInstance()->Start({ 0.f,0.f }, Fade::kFadeColor_, 20u);
}

void GameOverScene::OnExit() {
}

void GameOverScene::Update() {
	const float deltaTime = ImGui::GetIO().DeltaTime;
	if (isChangeSceneCall_) {
		sceneManager_->ChangeScene(new TitleScene, 60);
	}

	skyCylinder_->SetChangeSceneCall(isChangeSceneCall_);
	if (skyCylinder_) {
		skyCylinder_->Update(deltaTime);
	}

	titleLogo_->SetIsChangeSceneCall(isChangeSceneCall_);
	titleLogo_->Update(deltaTime);

	pressSprite_->SetIsChangeSceneCall(isChangeSceneCall_);
	pressSprite_->Update(deltaTime);

	if (input_->GetXInput()->IsTrigger(KeyCode::RIGHT_SHOULDER) ||
		input_->GetDirectInput()->IsTrigger(DIK_SPACE)) {
		isChangeSceneCall_ = true;
	}
}

void GameOverScene::Draw() {
	DirectXCommon* const dxCommon = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* const commandList = dxCommon->GetCommandList();

#pragma region 背面スプライト

	Sprite::StartDraw(commandList);

	// スプライトの描画

	Sprite::EndDraw();

#pragma endregion


	dxCommon->CrearDepthBuffer();

#pragma region モデル描画

	Model::StartDraw(commandList);

	light_->SetLight(commandList);
	if (skyCylinder_) {
		skyCylinder_->Draw(camera_);
	}

	Model::EndDraw();

#pragma endregion

#pragma region 前面スプライト

	Sprite::StartDraw(commandList);

	// スプライトの描画
	titleLogo_->Draw();
	pressSprite_->Draw();

	Fade::GetInstance()->Draw();

	Sprite::EndDraw();

#pragma endregion

}
