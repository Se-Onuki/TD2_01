#pragma once

#include "../../Engine/DirectBase/Render/Camera.h"

class FollowCamera
{
public:
	FollowCamera() = default;
	~FollowCamera() = default;

private:
	Camera<Render::CameraType::Projecction> camera_;

};