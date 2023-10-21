#include "SceneManager.h"

SceneManager *IScene::sceneManager_ = nullptr;
DirectResourceLeakChecker SceneManager::leakChecker_{};
void SceneManager::Init() {
	IScene::sceneManager_ = this;
	transitionTimer_.Clear();
}

void SceneManager::Cancel() {
	nextScene_.reset();
	transitionTimer_.Clear();
}

void SceneManager::SetScene(IScene *const nextScene) {
	if (nextScene == nullptr) return;
	if (currentScene_) {
		// 遷移前のシーンの退室処理
		currentScene_->OnExit();
	}
	// 保持するシーンのキーとポインタを更新
	currentScene_.reset(nextScene);
	currentScene_->OnEnter();
}

void SceneManager::ChangeScene(IScene *const nextScene, const int transitionTime) {
	// もし、次のシーンがあったらキャンセル
	if (nextScene_ != nullptr) {
		delete nextScene;
		return;
	}
	// 次のシーンのポインタを保存
	nextScene_.reset(nextScene);
	// 遷移タイマーを開始
	transitionTimer_.Start(transitionTime);
}

void SceneManager::Update() {
	if (transitionTimer_.Update() && transitionTimer_.IsFinish()) {
		if (sceneLoadThread_.joinable()) {
			sceneLoadThread_.join();
		}
		SetScene(nextScene_.release());
	}

	if (currentScene_) {
		currentScene_->Update();
	}
}

void SceneManager::Draw() const {
	if (currentScene_) {
		currentScene_->Draw();
	}
}