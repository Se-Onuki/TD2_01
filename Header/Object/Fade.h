#pragma once
#include <stdint.h>
#include <memory>

#include "../../Utils/SoLib/SoLib_Timer.h"
#include "../../Utils/SoLib/SoLib_Easing.h"

#include "../../Utils/Math/Vector2.h"
#include "../../Utils/Math/Vector4.h"
#include "../../Engine/DirectBase/2D/Sprite.h"

class Fade {
	Fade() = default;
	Fade(const Fade &) = delete;
	Fade &operator=(const Fade &) = delete;
	~Fade() = default;
public:

	static Fade *const GetInstance() {
		static Fade instance{};
		return &instance;
	}

	static void StaticInit();

	void SetState(const Vector2 &pos, const Vector4 &alpha);

	void Start(const Vector2 &targetPos, const Vector4 &targetAlpha, int goalTime);
	void Update();
	void Draw();

	void SetEaseFunc(float (*easeFunc)(float)) { easeFunc_ = easeFunc; }

	Sprite *const GetSprite() { return sprite_.get(); }
	const SoLib::Timer *const GetTimer() { return timer_.get(); }

	static const Vector4 kFadeColor_;

private:

	Vector2 targetPos_{};	// 移動後の座標
	Vector2 originalPos_{};	// 移動前の座標

	Vector4 targetColor_{};	// 変更後の色
	Vector4 originalColor_{};	// 変更前の色

	/// @brief イージング関数の関数ポインタ
	float (*easeFunc_)(float) = SoLib::easeLinear;

	std::unique_ptr<SoLib::Timer> timer_ = nullptr;
	std::unique_ptr<Sprite> sprite_ = nullptr;
};