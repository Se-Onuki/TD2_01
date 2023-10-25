#include "SoulComp.h"

#include <format>

#include "../../../Engine/DirectBase/Model/ModelManager.h"
#include "../../Object/GameManager.h"
#include "OrbComp.h"
#include "EnemyComp.h"

#include "../../../Utils/SoLib/SoLib_ImGui.h"

Entity *SoulComp::pOrb_ = nullptr;

void SoulComp::Init() {
	// オーブのいる位置
	Vector3 orbWorldPos = pOrb_->GetWorldPos();
	// 魂自身がいる位置
	const Vector3 selfWorldPos = object_->GetWorldPos();
	orbWorldPos.z = selfWorldPos.z;
	orbWorldPos.y = selfWorldPos.y;

	// エネルギーの計算
	energy_ = 7.f - (orbWorldPos - selfWorldPos).Length() / 0.2f;
	if (energy_ < 0.2f) { energy_ = 0.2f; }

}

void SoulComp::ModelInit() {
	int stunEnemyCount = EnemyComp::GetStunSum();
	//int enemySum = EnemyComp::GetEnemySum();
	if (isStun_) {
		if (stunEnemyCount >= 2) {
			modelComp_ = object_->AddComponent<ModelComp>();
			auto *const orbModel = ModelManager::GetInstance()->GetModel("Gold_Soul");
			modelComp_->AddBone("Gold_Soul", orbModel);
			energy_ *= 3.0f;
		}
		else if (stunEnemyCount == 1) {
			modelComp_ = object_->AddComponent<ModelComp>();
			auto *const orbModel = ModelManager::GetInstance()->GetModel("Red_Soul");
			modelComp_->AddBone("Red_Soul", orbModel);
			energy_ *= 2.0f;
		}

	}
	else if (!isStun_) {
		modelComp_ = object_->AddComponent<ModelComp>();
		auto *const orbModel = ModelManager::GetInstance()->GetModel("Purple_Soul");
		modelComp_->AddBone("Purple_Soul", orbModel);
	}

	if (energy_ >= 5.0f) {
		modelComp_->object_->transform_.scale = { 0.9f, 0.9f, 0.9f };
	}
	else if (energy_ >= 3.0f) {
		modelComp_->object_->transform_.scale = { 0.7f, 0.7f, 0.7f };

	}
	else if (energy_ >= 1.0f) {
		modelComp_->object_->transform_.scale = { 0.5f, 0.5f, 0.5f };
	}

}

void SoulComp::Update(float deltaTime) {

	// オーブのいる位置
	const Vector3 &orbWorldPos = pOrb_->GetWorldPos();
	// 魂自身がいる位置
	Vector3 selfWorldPos = object_->GetWorldPos();

	// オーブと魂の距離
	Vector3 distance = orbWorldPos - selfWorldPos;

#ifdef _DEBUG
	ImGui::Text("%s", std::format("energy : {:4.2} / Length : {:3.2} ", selfWorldPos.x, selfWorldPos.y, selfWorldPos.z, distance.Length(), energy_).c_str());
	//ImGui::Text("%f", pOrb_->GetComponent<OrbComp>)
#endif // _DEBUG

	// もし距離が近くなったら
	if (distance.Length() <= 3.0f) {
		object_->SetActive(false);
	}

	// 単位化して速度を合わせる
	const float speed = 2.0f;
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
