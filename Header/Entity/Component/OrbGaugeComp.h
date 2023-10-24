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

	//// エネルギー
	//float energy_ = 0.f;
	//// エネルギーの限界量
	//VariantItem<float> vMaxEnergy_{ "MaxEnergy", 10.f };
};