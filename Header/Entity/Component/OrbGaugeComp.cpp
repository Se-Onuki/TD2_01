#include "OrbGaugeComp.h"
#include "OrbComp.h"
#include <format>

#include "../../../Engine/DirectBase/Model/ModelManager.h"
//#include "../../Object/GameManager.h"

#include "../../../Utils/SoLib/SoLib_ImGui.h"
#include "../../../Utils/SoLib/SoLib.h"
Entity *OrbGaugeComp::pOrb_ = nullptr;

void OrbGaugeComp::Init() {
	modelComp_ = object_->AddComponent<ModelComp>();
	auto *const orbModel = ModelManager::GetInstance()->GetModel("OrbGauge");
	modelComp_->AddBone("OrbGauge", orbModel);
	modelComp_->object_->transform_.scale = { 3.0f, 3.0f, 3.0f };
	modelComp_->object_->transform_.translate.y = 5.0f;
	modelComp_->object_->transform_.translate.z = 10.0f;
}

void OrbGaugeComp::Update(float deltaTime) {
	auto *const orbComp = pOrb_->GetComponent<OrbComp>();
	object_->transform_.translate.y = pOrb_->transform_.translate.y;

	// 今のエネルギー情報を取得
	energyProgress = orbComp->GetProgress();
	Vector3 energy;
	// もし前フレーム時のエネルギー量と違ったら
	if (preEnergyProgress != energyProgress) {
		// ゲージ上がりフラグを有効に
		isGaugeUp = true;
	}

#ifdef _DEBUG
	ImGui::Text("%f", easingT);
#endif // _DEBUG
	// もしフラグがオンなら
	if (isGaugeUp) {
		// はじめとおわりを設定
		float startEnergyValue = 4.8f * preEnergyProgress;
		float endEnergyValue = 4.8f * energyProgress;

		// 実際の値を計算
		energyValue = startEnergyValue + ((endEnergyValue - startEnergyValue) * SoLib::easeInOutQuint(easingT));

		// イージングの媒介変数を加算
		if (easingT >= 1.0f) {
			easingT = 1.0f;
			isGaugeUp = false;
		}
		else {
			easingT += 1.2f * deltaTime;
		}

	}


	// 先ほど計算した値を設定
	energy = { energyValue, energyValue, energyValue };
	modelComp_->object_->transform_.scale = energy;

	// もしゲージ上がりフラグが無効なら
	if (!isGaugeUp) {
		// 前フレームの情報を保持
		preEnergyProgress = energyProgress;
		// イージング用媒介変数を初期化
		easingT = 0.0f;
	}
}

void OrbGaugeComp::Reset()
{
}

void OrbGaugeComp::SetOrbComp(Entity *orbComp) {
	pOrb_ = orbComp;
}
