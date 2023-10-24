#include "TitleScene.h"

#include <imgui.h>
#include "GameScene.h"
#include "../Engine/DirectBase/Base/DirectXCommon.h"
#include "../Engine/DirectBase/Model/ModelManager.h"
//#include "../Header/Entity/Component/SkyCylinderComp.h"

bool TitleScene::isChangeSceneCall_ = false;

TitleScene::TitleScene() {
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

}

TitleScene::~TitleScene() {
}

void TitleScene::OnEnter() {

	light_.reset(DirectionLight::Create());
	camera_.translation_ = Vector3{ 0.f, 10.f, -30.f };
	camera_.UpdateMatrix();
	auto* const modelManager = ModelManager::GetInstance();
	modelManager->AddModel("skyCylinder", Model::LoadObjFile("", "skyCylinder.obj"));

	isChangeSceneCall_ = false;


	titleLogo_ = std::make_unique<TitleLogo>();
	titleLogo_->Init("TitleLogo.png");
	pressSprite_ = std::make_unique<PressSprite>();
	pressSprite_->Init();

#pragma region SkyCylinderComp

	skyCylinder_ = std::make_unique<SkyCylinder>();
	//skyCylinder_->AddComponent<SkyCylinderComp>();
	skyCylinder_->Init("skyCylinder");

#pragma endregion

}

void TitleScene::OnExit() {
}

void TitleScene::Update() {
	const float deltaTime = ImGui::GetIO().DeltaTime;
	if (isChangeSceneCall_) {
		sceneManager_->ChangeScene(new GameScene, 60);
	}

	/*if (!skyCylinder_->GetActive()) {
		skyCylinder_.reset();
	}*/	
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

void TitleScene::Draw() {
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
	if (skyCylinder_) {
		skyCylinder_->Draw(camera_);
	}

	// モデルの描画

	Model::EndDraw();

#pragma endregion

#pragma region 前面スプライト

	Sprite::StartDraw(commandList);

	titleLogo_->Draw();
	pressSprite_->Draw();

	Sprite::EndDraw();

#pragma endregion

}
