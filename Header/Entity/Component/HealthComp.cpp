#include "HealthComp.h"
#include <algorithm>

void HealthComp::Reset() {
	nowHealth_ = maxHealth_;
}

void HealthComp::Update(float deltaTime) {
	if (nowHealth_ <= 0.f) {
		object_->SetActive(false);
	}
	deltaTime;
}

void HealthComp::SetMaxHealth(const float value) {
	maxHealth_ = value;
}

void HealthComp::AddHealth(const float value) {
	// 0から最大値に収めつつ、体力を変更する。
	nowHealth_ = std::clamp(nowHealth_ + value, 0.f, maxHealth_);
}
