#pragma once

#include <memory>
#include <list>

#include "../../Engine/DirectBase/2D/Sprite.h"
#include "../../Engine/DirectBase/Model/Model.h"
#include "../Entity/Entity.h"
#include "MapChip.h"

#include "../Entity/Component/Collider.h"
#include "FollowCamera.h"

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

	void Draw() const;

	void ImGuiWidget();

	const Vector2 kMapCentor_ = Vector2{ 1280.f, 720.f } / 2.f;

	void AddEnemy(const Vector3 &pos = Vector3::zero);

	void AddSoul(const Vector3& pos = Vector3::zero);

	static std::unique_ptr<Entity> orb_;

	const auto *const GetCamera() const { return followCamera_->GetCamera(); }

private:

	std::list<std::unique_ptr<Entity>> enemys_;

	std::list<std::unique_ptr<Entity>> souls_;

	std::unique_ptr<Entity> spring_ = nullptr;

	MapChip *mapChip_ = nullptr;

	std::unique_ptr<FollowCamera> followCamera_ = nullptr;

	CollisionManager *collisionManager_ = nullptr;
};
