#pragma once

#include <memory>
#include <list>
#include <sstream>

#include "../../Engine/DirectBase/2D/Sprite.h"
#include "../../Engine/DirectBase/Model/Model.h"
#include "../Entity/Entity.h"
#include "MapChip.h"

#include "../Object/SkyCylinder.h"
#include "../Entity/Component/Collider.h"
#include "FollowCamera.h"
#include "../Object/RemainEnemy.h"
#include "../Object/RemainWave.h"
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

	void Exit();

	void Update(const float deltaTime);

	void Draw() const;

	void Draw2D() const;

	void ImGuiWidget();

	const Vector2 kMapCentor_ = Vector2{ 1280.f, 720.f } / 2.f;

	void AddEnemy(const Vector3 &pos = Vector3::zero);

	void AddSoul(const Vector3 &pos = Vector3::zero, bool isStun = false);

	void AddPlayer();

	const auto *const GetPlayer() const { return spring_.get(); }

	const auto *const GetCamera() const { return followCamera_->GetCamera(); }

	const bool GetIsClear() { return isClear_; }
	const bool GetIsFinish() { return isFinish_; }

	void WaveChange();
	void WaveEnemySet(int wave);
private:

	std::list<std::unique_ptr<Entity>> enemys_;

	std::list<std::unique_ptr<Entity>> souls_;

	std::unique_ptr<Entity> spring_ = nullptr;

	MapChip *mapChip_ = nullptr;

	std::unique_ptr<FollowCamera> followCamera_ = nullptr;

	std::unique_ptr<Entity> orb_;
	std::unique_ptr<Entity> orbGauge_;
	std::unique_ptr<SkyCylinder> skyCylinder_ = nullptr;

	CollisionManager *collisionManager_ = nullptr;

	std::unique_ptr<Entity> debugSpawn_ = nullptr;

	std::unique_ptr<RemainEnemy> remainEnemy_ = nullptr;
	std::unique_ptr<RemainWave> remainWave_ = nullptr;

	Model *orbModel_ = nullptr;

	const float slowCount = 1.0f;
	SoLib::RealTimer slowmotionTimer_;

	// ウェーブ毎の敵の最大数
	uint32_t perWave_MaxEnemy_ = 0;

	int wave_ = 0;
	const int maxWave_ = 6;
	bool isClear_ = false;
	bool isFinish_ = false;

	uint32_t waveChangeSE_ = 0u;

	const Vector3 kDefaultSpawn_{ 0.f,10.f,0.f };

	Vector3 spawnPos_ = kDefaultSpawn_;

};
