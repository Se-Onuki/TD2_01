#include "GameScene.h"
#include "../externals/imgui/imgui.h"
#include "../DirectBase/Base/DirectXCommon.h"
#include "TitleScene.h"
#include "../DirectBase/Input/Input.h"

GameScene::GameScene() {
	bgm_ = Audio::GetInstance()->LoadWave("resources/Alarm01.wav");
	//soundData = SoundLoadWave("resources/Alarm01.wav");

}

GameScene::~GameScene() {
	//Audio::GetInstance()->Finalize();
	//soundData.Unload();
}

void GameScene::OnEnter()
{

	light_.reset(DirectionLight::Create());
	//Audio::GetInstance()->PlayWave(soundData);
	Audio::GetInstance()->PlayWave(bgm_);
}

void GameScene::OnExit()
{
}

void GameScene::Update() {
	const DirectInput *const directInput = DirectInput::GetInstance();
	//const XInput *const xInput = XInput::GetInstance();
	if (directInput->IsPress(DIK_A)) {
		light_->ImGuiWidget();
	}

}

void GameScene::Draw()
{
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

	Model::EndDraw();

#pragma endregion

#pragma region 前面スプライト

	Sprite::StartDraw(commandList);

	// スプライトの描画

	Sprite::EndDraw();

#pragma endregion

}
