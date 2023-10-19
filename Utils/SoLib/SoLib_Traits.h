#pragma once

#include <stdint.h>

#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"

// ゲッターとセッターを同時に定義するマクロ
#define GetSet(HogeType, Hoge, hoge_) \
    const HogeType& Get##Hoge() const { return hoge_; } \
    void Set##Hoge(const HogeType& hoge) { hoge_ = hoge; }

namespace SoLib {

	/// @brief 非ポインタ型
	template<typename T>
	concept IsNotPointer = !std::is_pointer_v<T>;

	template <typename T>
	struct Traits;

	template<>
	struct Traits<int32_t> {
		static constexpr auto Name = "int32_t";
		static constexpr uint32_t Count = 1u;
		using baseType = int32_t;
	};

	template<>
	struct Traits<float> {
		static constexpr auto Name = "float";
		static constexpr uint32_t Count = 1u;
		using baseType = float;
	};

	template<>
	struct Traits<Vector2> {
		static constexpr auto Name = "Vector2";
		static constexpr uint32_t Count = 2u;
		using baseType = float;
	};

	template<>
	struct Traits<Vector3> {
		static constexpr auto Name = "Vector3";
		static constexpr uint32_t Count = 3u;
		using baseType = float;
	};

	template<>
	struct Traits<Vector4> {
		static constexpr auto Name = "Vector4";
		static constexpr uint32_t Count = 4u;
		using baseType = float;
	};

}