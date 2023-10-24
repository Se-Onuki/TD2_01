#include "SkyCylinder.h"
//#include "../../../Engine/DirectBase/Model/ModelManager.h"
//#include "../../Object/GameManager.h"
#include "../../Utils/SoLib/SoLib.h"

SkyCylinder::SkyCylinder()
{
}

SkyCylinder::~SkyCylinder()
{
}

void SkyCylinder::Init(const std::string &fileName) {
	worldTransform_.InitResource();

	//	modelComp_ = object_->AddComponent<ModelComp>();

	auto *const skyModel = ModelManager::GetInstance()->GetModel(fileName);

	worldTransform_.scale = { 120.0f, 120.0f , 120.0f };
	model_ = skyModel;
	//modelComp_->AddBone("SkyCylinderComp", skyModel);
	//modelComp_->object_->transform_.scale = { 120.0f, 120.0f, 120.0f };

}

void SkyCylinder::Update(float deltaTime) {


	if (changeSceneCall_) {
		// 位置のイージング
		pos_ = SoLib::Lerp(start_, end_, SoLib::easeInBack(t_easing_));

		// イージングの媒介変数の処理
		if (t_easing_ >= 1.0f) {
			t_easing_ = 1.0f;
		}
		else {
			t_easing_ += 1.0f * deltaTime;
		}

		// 位置を設定
		worldTransform_.translate = pos_;
	}

	worldTransform_.rotate.y += (0.2f * deltaTime);
	worldTransform_.UpdateMatrix();
	preChangeSceneCall_ = changeSceneCall_;
}

void SkyCylinder::Draw(const Camera<Render::CameraType::Projecction> &camera) {
	model_->Draw(worldTransform_, camera);
}

void SkyCylinder::Reset() {
}
