#include "Rigidbody.h"
#include "../../Object/MapChip.h"

const float Rigidbody::deltaTime_ = 1.f / 60.f;

void Rigidbody::Init() {
	velocity_ = {};
	acceleration_ = {};
}

void Rigidbody::Update(float deltaTime) {

	const Vector3 beforePos = object_->transform_.translate;
	const Vector3 &afterPos = object_->transform_.translate;

	velocity_ += acceleration_;
	Vector3 fixVelocity = velocity_ * deltaTime * object_->timeScale_;

	// 最大速度のポインタ(配列として扱う)
	const float *const maxSpeedPtr = reinterpret_cast<float *>(&maxSpeed_);
	// 修正前の座標のポインタ
	float *const fixVelocityPtr = reinterpret_cast<float *>(&fixVelocity);
	// 各要素をclampする(最大速度が負数なら無効化)
	for (uint32_t i = 0u; i < 3u; ++i) {
		if (maxSpeedPtr[i] > 0.f) {
			fixVelocityPtr[i] = std::clamp(fixVelocityPtr[i], -maxSpeedPtr[i], maxSpeedPtr[i]);
		}
	}

	object_->transform_.translate += fixVelocity;

	isGround_ = false;


	//// y座標
	//float &valueY = object_->transform_.translate.y;
	//if (valueY <= radius_) {
	//	isGround_ = true; // 地面以下だった場合地面にいる
	//	valueY = radius_;
	//}

	if (hasCollider_) {
		const Vector3 hitPos = MapChip::GetInstance()->HitMap(beforePos, afterPos, radius_);

		if (hitPos.y != afterPos.y) {
			if (velocity_.y < 0.f) {
				isGround_ = true;
			}
			velocity_.y = 0.f;
		}

		//if (std::abs(hitPos.x - afterPos.x) < 0.001f) {
		//	velocity_.x = 0.f;
		//}
		if (hitPos != afterPos) {
			object_->transform_.translate = hitPos;
		}
	}

	if (isGround_) {
		velocity_.x = 0.f;

		if (velocity_.y < 0.f) {
			velocity_.y = 0.f;
		}
	}


	acceleration_ = {};
}
