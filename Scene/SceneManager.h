#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include "../Utils/SoLib/SoLib_Timer.h"
//#include "Scene.hpp"

class SceneManager;
class Entity;

class IScene {
public:
	IScene() = default;
	virtual ~IScene() = default;

	virtual void OnEnter() = 0;	// シーン入室時に一度走る
	virtual void OnExit() = 0;	// シーン退室時に一度走る

	virtual void Update() = 0;	// 更新処理
	virtual void Draw() = 0;	// 描画処理

	static SceneManager *sceneManager_;

	/// 今後実装
	// void AddObject(Object*const object);
	// std::list<std::unique_ptr<Object>> objectList_;
};

template <typename T>
concept IsSceneClass = std::derived_from<T, IScene>;

class SceneManager {
private:

	SoLib::Timer transitionTimer_{};
	// 現在読み込んでいるシーン
	std::unique_ptr<IScene> currentScene_ = nullptr;
	// 次に遷移するシーン
	std::unique_ptr<IScene> nextScene_ = nullptr;

	std::thread sceneLoadThread_;

	SceneManager() = default;
	SceneManager(const SceneManager &) = delete;
	SceneManager operator=(const SceneManager &) = delete;
	~SceneManager() = default;

public:

	void Init();


	/// @brief インスタンスの取得
	/// @return インスタンスの実体
	inline static SceneManager *const GetInstance() {
		static SceneManager instance;
		return &instance;
	}

	void Cancel();


	/// @brief シーン遷移
	/// @param name 遷移先のシーン
	void SetScene(IScene *const nextScene);


	/// @brief シーン遷移
	/// @param name 遷移先の名前キー
	/// @param transitionTime 必要とする時間
	void ChangeScene(IScene *const nextScene, const int transitionTime);


	/// @brief シーン遷移
	/// @param name 遷移先の名前キー
	/// @param transitionTime 必要とする時間
	template <IsSceneClass T>
	void ChangeScene(const int transitionTime);

	/// @brief シーン遷移
	/// @param name 遷移先の名前キー
	/// @param transitionTime 必要とする時間
	template <IsSceneClass T>
	void SetNextScene();


	/// @brief シーンの更新
	void Update();


	/// @brief シーンの描画
	void Draw() const;
};

template<IsSceneClass T>
inline void SceneManager::ChangeScene(const int transitionTime) {
	// もし、次のシーンがあったらキャンセル
	if (nextScene_ != nullptr || sceneLoadThread_.joinable()) { return; }
	// 遷移タイマーを開始
	transitionTimer_.Start(transitionTime);

	sceneLoadThread_ = std::thread([this]() { SetNextScene<T>(); });
}

template<IsSceneClass T>
inline void SceneManager::SetNextScene() {
	// 次のシーン
	T *const nextScene = new T{};

	// 次のシーンのポインタを保存
	nextScene_.reset(nextScene);
}