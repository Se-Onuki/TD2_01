#pragma once
#include "../Entity.h"
#include "ModelComp.h"
#include "../../../Engine/DirectBase/File/VariantItem.h"

class OrbGaugeComp :public IComponent {
public:
	using IComponent::IComponent;
	~OrbGaugeComp() = default;

	void Init() override;
	void Update(float deltaTime) override;

	void Reset() override;
	static void SetOrbComp(Entity* orbComp);

private:
	// モデル
	ModelComp* modelComp_ = nullptr;
	static Entity* pOrb_;

	float energyProgress = 0.0f;
	float preEnergyProgress = energyProgress;
	float easingT = 0.0f;
	float energyValue;
	bool isGaugeUp = false;;
	//// エネルギー
	//float energy_ = 0.f;
	//// エネルギーの限界量
	//VariantItem<float> vMaxEnergy_{ "MaxEnergy", 10.f };
};