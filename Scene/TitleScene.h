#pragma once
#include "SceneManager.h"

#include "../Engine/DirectBase/3D/DirectionLight.h"
#include "../Engine/DirectBase/2D/Sprite.h"

#include "../Engine/DirectBase/Input/Input.h"
#include "../Engine/DirectBase/Base/Audio.h"
#include "../Header/Object/TitleLogo.h"

class TitleScene : public IScene {
public:
	TitleScene();
	~TitleScene();

	void OnEnter() override;
	void OnExit() override;

	void Update() override;
	void Draw() override;
public:

private:

	Input *input_ = nullptr;
	Audio *audio_ = nullptr;

	bool isChangeSceneCall_ = false;

	std::unique_ptr<DirectionLight> light_ = nullptr;

	std::unique_ptr<TitleLogo> titleLogo_ = nullptr;

	std::unique_ptr<Sprite> pressSprite_ = nullptr;
};