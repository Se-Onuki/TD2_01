#include "SoLib_Timer.h"
#include <algorithm>

namespace SoLib {
	bool Timer::Update() {
		if (isFinish_) {
			isActive_ = false;
			return false;
		}
		AddFlame();
		if (nowFlame_ >= goalFlame_) {
			isFinish_ = true;
		}
		return true;
	}

	void Timer::AddFlame() {
		nowFlame_++;
	}

	void Timer::Start() {
		nowFlame_ = 0u;
		isFinish_ = false;
		isActive_ = true;
	}

	void Timer::Start(uint32_t goal) {
		Start();
		SetGoal(goal);
	}

	void Timer::Clear() {
		nowFlame_ = 0u;
		isFinish_ = true;
		isActive_ = false;
		goalFlame_ = 0u;
	}

	float Timer::GetProgress() const {
		return std::clamp(static_cast<float>(nowFlame_) / goalFlame_, 0.f, 1.f);
	}
	bool RealTimer::Update(float deltaTime) {
		if (isFinish_) {
			isActive_ = false;
			return false;
		}
		AddFlame(deltaTime);
		if (nowFlame_ >= goalFlame_) {
			isFinish_ = true;
		}
		return true;
	}

	void RealTimer::AddFlame(float deltaTime) {
		nowFlame_ += deltaTime;
	}

	void RealTimer::Start() {
		nowFlame_ = 0.f;
		isFinish_ = false;
		isActive_ = true;
	}

	void RealTimer::Start(float goal) {
		Start();
		SetGoal(goal);
	}

	void RealTimer::Clear() {
		nowFlame_ = 0u;
		isFinish_ = true;
		isActive_ = false;
		goalFlame_ = 0u;
	}

	float RealTimer::GetProgress() const {
		return std::clamp(nowFlame_ / goalFlame_, 0.f, 1.f);
	}
}