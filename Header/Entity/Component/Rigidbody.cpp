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
	object_->transform_.translate += velocity_ * deltaTime * object_->timeScale_;

	isGround_ = false;


	// y座標
	float &valueY = object_->transform_.translate.y;
	if (valueY <= radius_) {
		isGround_ = true; // 地面以下だった場合地面にいる
		valueY = radius_;
	}

	if (hasCollider_) {
		const Vector3 hitPos = MapChip::GetInstance()->HitMap(beforePos, afterPos, radius_);

		if (hitPos.y != afterPos.y) {
			isGround_ = true;
		}
		if (hitPos != afterPos) {
			object_->transform_.translate = hitPos;
		}
	}

	if (isGround_) {

		if (velocity_.y < 0.f) {
			velocity_.y = 0.f;
		}
	}


	acceleration_ = {};
}
