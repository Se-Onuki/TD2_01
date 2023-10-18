#pragma once

#include "../../Engine/DirectBase/Render/Camera.h"
#include "../Entity/Entity.h"
#include "../Entity/Component/SpringObjectComp.h"
class FollowCamera {

	const Entity *target_ = nullptr;
	const SpringObjectComp *springComp = nullptr;
	Vector3 offset = { 0.f, 5.f, -50.f };
public:
	FollowCamera() = default;
	~FollowCamera() = default;

	void Reset();
	void Update(float deltaTime);

	void SetSpring(const Entity *spring);

	const auto *const GetCamera() const { return &camera_; }

private:
	Camera<Render::CameraType::Projecction> camera_;

};