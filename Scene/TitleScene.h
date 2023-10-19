#pragma once
#include "SceneManager.h"

#include "../Engine/DirectBase/3D/DirectionLight.h"
#include "../Engine/DirectBase/2D/Sprite.h"
#include "../Engine/DirectBase/Render/Camera.h"

#include "../Engine/DirectBase/Input/Input.h"
#include "../Engine/DirectBase/Base/Audio.h"
#include "../Header/Object/TitleLogo.h"
#include "../Header/Object/PressSprite.h"

class TitleScene : public IScene {
public:
	TitleScene();
	~TitleScene();

	void OnEnter() override;

	void OnExit() override;

	void Update() override;
	
	void Draw() override;
public:
	static bool isChangeSceneCall_;

private:

	Input *input_ = nullptr;
	Audio *audio_ = nullptr;


	std::unique_ptr<DirectionLight> light_ = nullptr;

	std::unique_ptr<TitleLogo> titleLogo_ = nullptr;

	std::unique_ptr<PressSprite> pressSprite_ = nullptr;

	Camera<Render::CameraType::Projecction> camera_;

	std::unique_ptr<Entity> skyCylinder_ = nullptr;

};