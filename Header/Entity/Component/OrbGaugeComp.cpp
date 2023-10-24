#include "OrbGaugeComp.h"
#include "OrbComp.h"
#include <format>

#include "../../../Engine/DirectBase/Model/ModelManager.h"
//#include "../../Object/GameManager.h"

#include "../../../Utils/SoLib/SoLib_ImGui.h"

Entity* OrbGaugeComp::pOrb_ = nullptr;

void OrbGaugeComp::Init() {
	modelComp_ = object_->AddComponent<ModelComp>();
	auto* const orbModel = ModelManager::GetInstance()->GetModel("OrbGauge");
	modelComp_->AddBone("OrbGauge", orbModel);
	modelComp_->object_->transform_.scale = { 3.0f, 3.0f, 3.0f };
	modelComp_->object_->transform_.translate.y = 5.0f;
	modelComp_->object_->transform_.translate.z = 10.0f;

}

void OrbGaugeComp::Update(float ) {
	auto* const orbComp = pOrb_->GetComponent<OrbComp>();
	float energyProgress = orbComp->GetProgress();
	Vector3 energy = { 4.8f * energyProgress, 4.8f * energyProgress, 4.8f * energyProgress };

	modelComp_->object_->transform_.scale = energy;


}

void OrbGaugeComp::Reset()
{
}

void OrbGaugeComp::SetOrbComp(Entity* orbComp) {
	pOrb_ = orbComp;
}
