#include "FollowCamera.h"

#include "../Object/GameManager.h"
#include "../../Utils/SoLib/SoLib_Lerp.h"
#include "../../Engine/DirectBase/File/GlobalVariables.h"


void FollowCamera::Reset() {
	ApplyVariables(groupName_.c_str());

	const auto *const target = GameManager::GetInstance()->GetPlayer();
	if (target) {
		camera_.translation_ = offset + target->transform_.translate;
	}
	AddVariable(groupName_.c_str());
}

void FollowCamera::Update([[maybe_unused]] float deltaTime) {
	ApplyVariables(groupName_.c_str());
	static const auto &ClampFunc = [this](float value) {return  std::clamp(value, vMinPos.GetItem(), MapChip::kMapHight_ * 2.f); };

	const auto *const target = GameManager::GetInstance()->GetPlayer();

	camera_.translation_.y = ClampFunc(camera_.translation_.y);
	if (target) {
		float targetHight = ClampFunc((offset + target->transform_.translate).y);
		camera_.translation_.y = SoLib::Lerp(camera_.translation_.y, targetHight, vFollowSpeed);
	}
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

