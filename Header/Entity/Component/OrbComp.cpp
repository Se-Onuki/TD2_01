#include "OrbComp.h"

#include "../../../Engine/DirectBase/Model/ModelManager.h"


void OrbComp::Init() {

	modelComp_ = object_->AddComponent<ModelComp>();
	
	auto* const orbModel = ModelManager::GetInstance()->GetModel("Orb");
	modelComp_->AddBone("Orb", orbModel);
	
	//auto* const colliderComp = object_->AddComponent<ColliderComp>();
	//colliderComp->SetRadius(5.f);
	//colliderComp->SetCollisionAttribute(static_cast<uint32_t>(CollisionFilter::Ground));
	//colliderComp->SetCollisionMask(~static_cast<uint32_t>(CollisionFilter::Ground));

	// 仮で球の大きさを調整中
	modelComp_->object_->transform_.scale = { 5.0f, 5.0f, 5.0f };
	modelComp_->object_->transform_.translate.z = 20.0f;

}

void OrbComp::Reset() {
}
