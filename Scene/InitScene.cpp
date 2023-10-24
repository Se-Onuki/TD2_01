#include "InitScene.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "../Engine/DirectBase/Base/WinApp.h"

void InitScene::OnEnter() {
	//auto gameScene = std::make_unique<GameScene>();
	GameScene::StartupLoad();

	sceneManager_->ChangeScene<TitleScene>(2);

	sprite_.reset(Sprite::Create());
	sprite_->SetScale({ WinApp::kWindowWidth,WinApp::kWindowHeight });
}

void InitScene::Draw() {
	DirectXCommon *const dxCommon = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList *const commandList = dxCommon->GetCommandList();

	Sprite::StartDraw(commandList);

	// スプライトの描画
	sprite_->Draw();

	Sprite::EndDraw();

}
