#pragma once
#include "SceneManager.h"

class InitScene : public IScene {
public:
	InitScene() = default;
	~InitScene() = default;

	void OnEnter() override;

	void OnExit() override {};

	void Update() override {};

	void Draw() override {};
public:
	static bool isChangeSceneCall_;

private:
};