#include "FollowCamera.h"
#include "../../Utils/SoLib/SoLib_Lerp.h"
#include "../../Engine/DirectBase/File/GlobalVariables.h"
void FollowCamera::Reset() {
	ApplyVariables(groupName_.c_str());
	if (target_) {
		camera_.translation_ = offset + target_->transform_.translate;
	}
	AddVariable(groupName_.c_str());
}

void FollowCamera::Update([[maybe_unused]] float deltaTime) {
	ApplyVariables(groupName_.c_str());
	static const auto &ClampFunc = [this](float value) {return  std::clamp(value, vMinPos.GetItem(), MapChip::kMapHight_ * 2.f); };

	float fixheight = ClampFunc(camera_.translation_.y);
	float targetHight = ClampFunc((offset + target_->transform_.translate).y);
	camera_.translation_.y = SoLib::Lerp(fixheight, targetHight, vFollowSpeed);
	//camera_.translation_.y = std::clamp(camera_.translation_.y, vMinPos.GetItem(), MapChip::kMapHight_ * 2.f);
	camera_.UpdateMatrix();
}

void FollowCamera::ApplyVariables(const char *const groupName) {
	GlobalVariables *const gVariable = GlobalVariables::GetInstance();
	const auto &cGroup = gVariable->GetGroup(groupName);

	cGroup >> vFollowSpeed;
	cGroup >> vMinPos;
}

void FollowCamera::AddVariable(const char *const groupName) const {
	GlobalVariables *const gVariable = GlobalVariables::GetInstance();
	auto &group = gVariable->GetGroup(groupName);

	group << vFollowSpeed;
	group << vMinPos;
}

void FollowCamera::SetSpring(const Entity *spring) {
	target_ = spring;
	springComp = spring->GetComponent<SpringObjectComp>();
}
