#pragma once
#include "SceneManager.h"

#include <memory>
#include "../Engine/DirectBase/2D/Sprite.h"

class InitScene : public IScene {
public:
	InitScene() = default;
	~InitScene() = default;

	void OnEnter() override;

	void OnExit() override {};

	void Update() override {};

	void Draw() override;
public:

private:
	std::unique_ptr<Sprite> sprite_ = nullptr;

};