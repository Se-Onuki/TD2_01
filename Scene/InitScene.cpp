#include "InitScene.h"
#include "GameScene.h"
#include "TitleScene.h"

void InitScene::OnEnter() {
	auto gameScene = std::make_unique<GameScene>();

	sceneManager_->ChangeScene<TitleScene>(1);
}
