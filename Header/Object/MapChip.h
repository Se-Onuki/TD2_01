#pragma once
#include <stdint.h>
#include <array>
#include "../../Utils/Math/Transform.h"
#include "../../Engine/DirectBase/Model/Model.h"

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
		kUnbreakable	// 破壊不可
	};

	struct ChipData {
		ChipState chipState_ = ChipState::kAir;
		std::unique_ptr<Transform> transform_;
		Model *model_;

		void Init();

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
		static MapChip instance{};
		return &instance;
	}

	void Init();

	void Draw(const Camera<Render::CameraType::Projecction> &camera) const;

	const MapArray &GetMapArray()const { return mapChip_; }

	Vector3 HitMap(const Vector3 &beforePos, const Vector3 &afterPos, float radius) const;

private:

	MapArray mapChip_ = {};

};
