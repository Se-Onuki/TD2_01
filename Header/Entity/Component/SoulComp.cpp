#include "SoulComp.h"

#include <format>

#include "../../../Engine/DirectBase/Model/ModelManager.h"
#include "../../Object/GameManager.h"
#include "OrbComp.h"

#include "../../../Utils/SoLib/SoLib_ImGui.h"

Entity *SoulComp::pOrb_ = nullptr;

void SoulComp::Init() {
	modelComp_ = object_->AddComponent<ModelComp>();
	auto *const orbModel = ModelManager::GetInstance()->GetModel("Soul");
	modelComp_->AddBone("Soul", orbModel);
	modelComp_->object_->transform_.scale = { 0.5f, 0.5f, 0.5f };

	// オーブのいる位置
	Vector3 orbWorldPos = pOrb_->GetWorldPos();
	// 魂自身がいる位置
	Vector3 selfWorldPos = object_->GetWorldPos();
	orbWorldPos.z = selfWorldPos.z;

	// 
	energy_ = 7.f - (orbWorldPos - selfWorldPos).Length() / 2.f;
	if (energy_ < 1.f) { energy_ = 1.f; }
}

void SoulComp::Update(float deltaTime) {

	// オーブのいる位置
	const Vector3 &orbWorldPos = pOrb_->GetWorldPos();
	// 魂自身がいる位置
	Vector3 selfWorldPos = object_->GetWorldPos();

	// オーブと魂の距離
	Vector3 distance = orbWorldPos - selfWorldPos;

#ifdef _DEBUG
	ImGui::Text("%s", std::format("soulPos : {:3.2}, {:3.2}, {:3.2} / Length : {:3.2} / energy : {:4.2}", selfWorldPos.x, selfWorldPos.y, selfWorldPos.z, distance.Length(), energy_).c_str());
#endif // _DEBUG

	// もし距離が近くなったら
	if (distance.Length() <= 3.0f) {
		object_->SetActive(false);
	}

	// 単位化して速度を合わせる
	distance.Nomalize();
	const float speed = 1.0f;
	distance = distance * speed * deltaTime;
	object_->transform_.translate += distance;
}

void SoulComp::Reset() {
}

void SoulComp::Destroy() {
	auto *const orbComp = pOrb_->GetComponent<OrbComp>();
	orbComp->AddEnergy(energy_);
}

void SoulComp::SetOrbComp(Entity *orbComp) {
	pOrb_ = orbComp;
}
