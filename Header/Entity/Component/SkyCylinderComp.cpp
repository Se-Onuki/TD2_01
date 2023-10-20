#include "SkyCylinderComp.h"
#include "../../../Engine/DirectBase/Model/ModelManager.h"
#include "../../Object/GameManager.h"
#include "../../../Utils/SoLib/SoLib.h"

void SkyCylinderComp::Init() {
	modelComp_ = object_->AddComponent<ModelComp>();
	auto* const skyModel = ModelManager::GetInstance()->GetModel("skyCylinder");
	modelComp_->AddBone("SkyCylinderComp", skyModel);
	modelComp_->object_->transform_.scale = { 120.0f, 120.0f, 120.0f };

	
}

void SkyCylinderComp::Update([[maybe_unused]]float deltaTime) {
	//if (TitleScene::isChangeSceneCall_) {
	//	// 位置のイージング
	//	pos_.x = start_.x + ((end_.x - start_.x) * SoLib::easeInBack(t_easing_));
	//	pos_.y = start_.y + ((end_.y - start_.y) * SoLib::easeInBack(t_easing_));
	//	pos_.z = start_.z + ((end_.z - start_.z) * SoLib::easeInBack(t_easing_));

	//	// イージングの媒介変数の処理
	//	if (t_easing_ >= 1.0f) {
	//		t_easing_ = 1.0f;
	//	}
	//	else {
	//		t_easing_ += 1.0f * deltaTime;
	//	}

	//	// 位置を設定
	//	object_->transform_.translate = pos_;
	//}

	//object_->transform_.rotate.y += (0.2f * deltaTime);

	//preChangeSceneCall_ = TitleScene::isChangeSceneCall_;
}

void SkyCylinderComp::Reset()
{
}
