#include "TitleScene.h"

#include <imgui.h>
#include "GameScene.h"
#include "../Engine/DirectBase/Base/DirectXCommon.h"
#include "../Engine/DirectBase/Model/ModelManager.h"

TitleScene::TitleScene() {
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
}

TitleScene::~TitleScene() {
}

void TitleScene::OnEnter() {

	light_.reset(DirectionLight::Create());

	titleLogo_ = std::make_unique<TitleLogo>();
	titleLogo_->OnEnter();

	timer_ = std::make_unique<SoLib::Timer>(60);	
	timer_->Start();

}

void TitleScene::OnExit() {
}

void TitleScene::Update() {
	timer_->Update();

	titleLogo_->Update();
	if (timer_->IsFinish()) {
		sceneManager_->ChangeScene(new GameScene, 60);
	}

	if (input_->GetXInput()->IsTrigger(KeyCode::RIGHT_SHOULDER)) {
		sceneManager_->ChangeScene(new GameScene, 60);
	}
}

void TitleScene::Draw() {
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

	// モデルの描画

	Model::EndDraw();

#pragma endregion

#pragma region 前面スプライト

	Sprite::StartDraw(commandList);
	if (!timer_->IsFinish()) {
		titleLogo_->Draw();
	}

	Sprite::EndDraw();

#pragma endregion

}
