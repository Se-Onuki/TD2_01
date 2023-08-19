#pragma once
#include "../../DirectBase/3D/ViewProjection/ViewProjection.h"
#include "../Math/Math.hpp"

struct Transform;
class Input;

class FollowCamera {
	ViewProjection viewProjection_;
	const Transform *target_ = nullptr;
	Vector3 rotate_ = {};
	const float cameraRotSpeed_ = 3.f * Angle::Dig2Rad;
	Input *input_ = nullptr;

public:
	FollowCamera() = default;
	~FollowCamera() = default;

	void Init();
	void Update();
	inline void SetTarget(const Transform *const target) {
		target_ = target;
	}

	const Matrix4x4 &GetViewMatrix() const;
	const Matrix4x4 &GetProjectionMatrix() const;

	const ViewProjection *const GetViewProjection() const;
};
