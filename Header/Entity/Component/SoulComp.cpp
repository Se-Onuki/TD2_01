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
	// オーブのいる位置
	Vector3 orbWorldPos = pOrb_->GetWorldPos();
	// 魂自身がいる位置
	Vector3 myWorldPos = object_->GetWorldPos();

	// オーブと魂の距離
	Vector3 distance = orbWorldPos - myWorldPos;
	
	// もし距離が近くなったら
	if (distance.Length() <= 3.0f) {
		object_->SetActive(false);
	}

	// 単位化して速度を合わせる
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
