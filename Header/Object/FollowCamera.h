#pragma once

#include "../../Engine/DirectBase/Render/Camera.h"
#include "../Entity/Entity.h"
#include "../Entity/Component/SpringObjectComp.h"

#include "../../Engine/DirectBase/File/VariantItem.h"
#include "../Object/MapChip.h"


class FollowCamera {

public:
	FollowCamera() = default;
	~FollowCamera() = default;

	void Reset();
	void Update(float deltaTime);

	/// @brief jsonからの読み込み
	/// @param groupName グループ名
	virtual void ApplyVariables(const char *const groupName);

	/// @brief jsonへの紐づけ
	/// @param groupName グループ名
	virtual void AddVariable(const char *const groupName) const;


	const auto *const GetCamera() const { return &camera_; }

	VariantItem<float> vFollowSpeed{ "FollowSpeed", 0.05f };
	VariantItem<float> vMinPos{ "MinPos", 10.f };

private:
	float ClampFunc(float value) { return  std::clamp(value, vMinPos.GetItem(), MapChip::kMapHight_ * 2.f); };
	Vector3 offset = { 0.f, 5.f, -50.f };
	const std::string groupName_ = "FollowCamera";
	Camera<Render::CameraType::Projecction> camera_;

};