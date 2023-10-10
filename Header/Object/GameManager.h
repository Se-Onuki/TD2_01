#pragma once

#include <memory>
#include "../../Engine/DirectBase/2D/Sprite.h"
#include "../../Engine/DirectBase/Model/Model.h"
#include "../Entity/Entity.h"
#include "MapChip.h"

class GameManager {
	GameManager() = default;
	GameManager(const GameManager &) = delete;
	GameManager &operator=(const GameManager &) = delete;
	~GameManager() = default;

public:
	static GameManager *const GetInstance() {
		static GameManager instance{};
		return &instance;
	}

	void Init();

	void Update(const float deltaTime);

	void Draw(const Camera<Render::CameraType::Projecction> &camera) const;

	void ImGuiWidget();

	const Vector2 kMapCentor_ = Vector2{ 1280.f, 720.f } / 2.f;



private:

	Entity spring_;
	MapChip *mapChip_ = nullptr;
	// Model *spring_ = nullptr;

};
