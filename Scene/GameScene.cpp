#include "GameScene.h"

#include <imgui.h>
#include "../Engine/DirectBase/Base/DirectXCommon.h"
#include "../Engine/DirectBase/Model/ModelManager.h"

#include "TitleScene.h"
#include "GameOverScene.h"
#include "GameClearScene.h"

#include "../Header/Entity/Component/ModelComp.h"
#include "../Utils/SoLib/SoLib_ImGui.h"

#include "../Header/Object/Fade.h"

uint32_t GameScene::BGMHandle_;
void GameScene::StartupLoad() {

	auto *const modelManager = ModelManager::GetInstance();
	auto *const sphere = modelManager->AddModel("Sphere", Model::LoadObjFile("", "sphere.obj"));
	sphere->materialMap_["Material"]->materialBuff_->color = Vector4{ 1.f,1.f,1.f,0.5f };
	sphere->materialMap_["Material"]->materialBuff_->emissive = Vector4::one;
	sphere->materialMap_["Material"]->blendMode_ = Model::BlendMode::kNormal;

	modelManager->AddModel("Spring", Model::LoadObjFile("Model/Player/", "player.obj"));
	modelManager->AddModel("Enemy", Model::LoadObjFile("Model/Enemy/", "enemy.obj"));

	auto *const orb = modelManager->AddModel("Orb", Model::LoadObjFile("Model/Orb/", "orb.obj"));
	orb->materialMap_["Material"]->blendMode_ = Model::BlendMode::kNormal;
	orb->materialMap_["Material"]->materialBuff_->color = { 1.0f,1.0f, 1.0f, 0.6f };
	orb->materialMap_["Material"]->materialBuff_->emissive = Vector4::one * 0.2f;

	auto *const orbGauge = modelManager->AddModel("OrbGauge", Model::LoadObjFile("Model/OrbGauge/", "orbGauge.obj"));
	orbGauge->materialMap_["Material"]->blendMode_ = Model::BlendMode::kNormal;
	orbGauge->materialMap_["Material"]->materialBuff_->emissive = Vector4::one * 0.2f;


	auto *const unbleakble = modelManager->AddModel("Unbreakable", Model::LoadObjFile("Model/Block/", "block.obj"));
	unbleakble->materialMap_["Material"]->texHandle_ = TextureManager::Load("Model/Block/unbreakableBlock_test.png");

	auto *const crack = modelManager->AddModel("Crack", Model::LoadObjFile("Model/Block/", "block.obj"));
	crack->materialMap_["Material"]->texHandle_ = TextureManager::Load("Model/Block/creakedBlock_test.png");

	auto *const box = modelManager->AddModel("Box", Model::LoadObjFile("Model/Block/", "block.obj"));
	box->materialMap_["Material"]->texHandle_ = TextureManager::Load("Model/Block/stoneBlock_test.png");

	modelManager->AddModel("Purple_Soul", Model::LoadObjFile("Model/Souls/purple_soul/", "purple_soul.obj"));
	modelManager->AddModel("Red_Soul", Model::LoadObjFile("Model/Souls/red_soul/", "red_soul.obj"));
	modelManager->AddModel("Gold_Soul", Model::LoadObjFile("Model/Souls/gold_soul/", "gold_soul.obj"));

	TextureManager::Load("backTexture.png");
	TextureManager::Load("UI/KeyInput.png");

	BGMHandle_ = Audio::GetInstance()->LoadWave("resources/Sounds/inGame.wav");
}

GameScene::GameScene() {
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ライトの生成
	light_.reset(DirectionLight::Create());

	camera_.translation_ = Vector3{ 0.f, 10.f, -30.f };
	camera_.UpdateMatrix();

	sprite_.reset(Sprite::Create(TextureManager::Load("backTexture.png")));
	sprite_->SetScale({ 1280.0f, 720.0f });
	sprite_->SetPosition({ 0.0f, 0.0f });

	keyHud_.reset(Sprite::Create(TextureManager::Load("UI/KeyInput.png")));
	keyHud_->SetScale(Vector2{ 714.f, 256.0f }*0.6f);
	keyHud_->SetPivot({ 0.f,1.f });
	keyHud_->SetPosition({ 16., 720.f });
	keyHud_->SetTexOrigin({ 0.f,128.f });
}

GameScene::~GameScene() {
	//audio_->StopWave(voiceBGMHandle_);

}

void GameScene::OnEnter() {

	gameManager_ = GameManager::GetInstance();
	gameManager_->Init();

	Fade::GetInstance()->Start({ 0.f,0.f }, Fade::kFadeColor_, 20u);

}

void GameScene::OnExit() {
	audio_->StopAllWave();

	gameManager_->Exit();
}

void GameScene::Update() {
	const float deltaTime = ImGui::GetIO().DeltaTime;
	// BGM再生
	if (audio_->IsPlaying(voiceBGMHandle_) == 0 || voiceBGMHandle_ == -1) {
		voiceBGMHandle_ = audio_->PlayWave(BGMHandle_, true, bolume);
	}

	ImGui::Begin("Camera");
	camera_.ImGuiWidget();
	ImGui::End();
	camera_.UpdateMatrix();

	keyHud_->ImGuiWidget();

	ImGui::Begin("Sphere");
	//	model_->ImGuiWidget();
	//	transform_.ImGuiWidget();
	ImGui::End();

	TextureManager::GetInstance()->ImGuiWindow();
	gameManager_->Update(deltaTime);

	gameManager_->ImGuiWidget();

	light_->ImGuiWidget();
	//if (/*input_->GetXInput()->IsTrigger(KeyCode::RIGHT_SHOULDER)*/
	//	input_->GetDirectInput()->IsTrigger(DIK_2)) {
	//	sceneManager_->ChangeScene<TitleScene>(30);
	//}
	//if (/*input_->GetXInput()->IsTrigger(KeyCode::RIGHT_SHOULDER) */
	//	input_->GetDirectInput()->IsTrigger(DIK_3)) {
	//	sceneManager_->ChangeScene<GameOverScene>(60);
	//}
	if (gameManager_->GetIsFinish()) {
		audio_->StopWave(voiceBGMHandle_);
		if (gameManager_->GetIsClear()) {
			sceneManager_->ChangeScene<GameClearScene>(60);
		}
		else {
			sceneManager_->ChangeScene<GameOverScene>(60);
		}
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
	keyHud_->Draw();

	Fade::GetInstance()->Draw();

	Sprite::EndDraw();

#pragma endregion

}