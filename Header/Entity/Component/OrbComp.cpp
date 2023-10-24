#include "OrbComp.h"

#include "../../../Engine/DirectBase/Model/ModelManager.h"


void OrbComp::Init() {

	modelComp_ = object_->AddComponent<ModelComp>();

	auto *const orbModel = ModelManager::GetInstance()->GetModel("Orb");
	modelComp_->AddBone("Orb", orbModel);
	//auto* const orbGaugeModel = ModelManager::GetInstance()->GetModel("OrbGauge");
	//gaugeModelComp_->AddBone("OrbGauge", orbGaugeModel);

	//auto* const colliderComp = object_->AddComponent<ColliderComp>();
	//colliderComp->SetRadius(5.f);
	//colliderComp->SetCollisionAttribute(static_cast<uint32_t>(CollisionFilter::Ground));
	//colliderComp->SetCollisionMask(~static_cast<uint32_t>(CollisionFilter::Ground));

	// 仮で球の大きさを調整中
	modelComp_->object_->transform_.scale = Vector3::one * 10;
	modelComp_->object_->transform_.translate.y = 5.0f;
	modelComp_->object_->transform_.translate.z = 10.0f;
	//gaugeModelComp_->object_->transform_.scale = { 4.8f, 4.8f, 4.8f };
	//gaugeModelComp_->object_->transform_.translate.z = 20.0f;

}

void OrbComp::Update(float)
{
}

void OrbComp::Reset() {
}
