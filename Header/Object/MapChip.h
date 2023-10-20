#pragma once
#include <stdint.h>
#include <array>
#include "../../Utils/Math/Transform.h"
#include "../../Engine/DirectBase/Model/Model.h"

#include "../../Engine/DirectBase/Base/LeakChecker.h"

class MapChip {

	MapChip() = default;
	MapChip(const MapChip &) = delete;
	MapChip &operator=(const MapChip &) = delete;
	~MapChip() = default;

public:

	enum class ChipState : uint32_t {
		kAir,	// 空白
		kBox,	// 箱
		kBarrier,		// バリア
		kCrack,			// ヒビ
		kUnbreakable	// 破壊不可
	};

	struct ChipData {
		ChipState chipState_ = ChipState::kAir;
		std::unique_ptr<Transform> transform_;
		Model *model_;

		void Init();

		void Exit();

		void Create(uint32_t x, uint32_t y);

		void Draw(const Camera<Render::CameraType::Projecction> &camera) const;

		ChipData &operator=(ChipState state);
		bool operator==(ChipState state) const { return chipState_ == state; }
	};

	static const uint32_t kMapWidth_ = 20u;
	static const uint32_t kMapHight_ = 30u;

	float kScale_ = 2.f;

	using MapArray = std::array<std::array<ChipData, kMapWidth_>, kMapHight_>;


public:

	static auto *const GetInstance() {
		static DirectResourceLeakChecker leakChacker{};
		static MapChip instance{};
		return &instance;
	}

	void Init();

	void Exit();

	/// @brief ヒビを入れる
	/// @param x ローカルx座標
	/// @param y ローカルy座標
	void SetCrack(uint32_t x, uint32_t y);

	/// @brief ヒビを破壊
	/// @param x ローカルx座標
	/// @param y ローカルy座標
	void SetBreak(uint32_t x, uint32_t y);

	void Draw(const Camera<Render::CameraType::Projecction> &camera) const;

	static Vector2 GlobalToLocal(const Vector3 &global) {
		const float xCenter = MapChip::kMapWidth_ / 2.f;
		return Vector2{ std::floor((global.x / 2.f) + xCenter), std::floor((global.y - 1.f) / 2.f) };
	}

	const ChipData &GetChipData(const Vector2 &vec);


	const MapArray &GetMapArray()const { return mapChip_; }

	Vector3 HitMap(const Vector3 &beforePos, const Vector3 &afterPos, float radius) const;

private:

	MapArray mapChip_ = {};

};
