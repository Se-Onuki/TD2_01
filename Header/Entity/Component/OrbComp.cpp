#include "OrbComp.h"

#include "../../../Engine/DirectBase/Model/ModelManager.h"


void OrbComp::Init() {

	modelComp_ = object_->AddComponent<ModelComp>();

	auto *const springModel = ModelManager::GetInstance()->GetModel("Gauge");
	modelComp_->AddBone("Body", springModel);
}

void OrbComp::Reset() {
}
