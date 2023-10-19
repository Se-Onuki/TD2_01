#include "FollowCamera.h"
#include "../../Utils/SoLib/SoLib_Lerp.h"
void FollowCamera::Reset() {
	if (target_) {
		camera_.translation_ = offset + target_->transform_.translate;
	}
}

void FollowCamera::Update([[maybe_unused]] float deltaTime) {
	camera_.translation_.y = SoLib::Lerp(camera_.translation_.y, (offset + target_->transform_.translate).y, 0.05f);
	camera_.UpdateMatrix();
}

void FollowCamera::SetSpring(const Entity *spring) {
	target_ = spring;
	springComp = spring->GetComponent<SpringObjectComp>();
}
