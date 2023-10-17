#include "SoulComp.h"
#include "../../../Engine/DirectBase/Model/ModelManager.h"
#include "../../Object/GameManager.h"


void SoulComp::Init() {
	modelComp_ = object_->AddComponent<ModelComp>();
	auto* const orbModel = ModelManager::GetInstance()->GetModel("Soul");
	modelComp_->AddBone("Soul", orbModel);
	modelComp_->object_->transform_.scale = { 0.5f, 0.5f, 0.5f };
	SetOrbComp(GameManager::orb_.get());
}

void SoulComp::Update(float) {
	Vector3 orbWorldPos = pOrb_->GetWorldPos();
	Vector3 myWorldPos = object_->GetWorldPos();

	Vector3 distance = orbWorldPos - myWorldPos;
	
	if (distance.Length() <= 3.0f) {
		object_->SetActive(false);
	}
	distance.Nomalize();
	const float speed = 0.02f;
	distance = distance * speed;
	object_->transform_.translate += distance;
}

void SoulComp::Reset() {
}

void SoulComp::SetOrbComp(Entity* orbComp) {
	pOrb_ = orbComp;
}
