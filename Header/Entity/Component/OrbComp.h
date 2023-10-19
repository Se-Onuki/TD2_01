#pragma once
#include "../Entity.h"
#include "ModelComp.h"
#include "../../../Engine/DirectBase/File/VariantItem.h"

class OrbComp :public IComponent {
public:
	using IComponent::IComponent;
	~OrbComp() = default;

	void Init() override;
	void Reset() override;

	void SetMaxEnergy(float value) { vMaxEnergy_ = value; }
	float GetMaxEnergy() const { return vMaxEnergy_; }

	void SetEnergy(float value) { energy_ = value; }
	void AddEnergy(float value) { energy_ += value; }

	float GetProgress() const { return energy_ / static_cast<float>(vMaxEnergy_); }

private:
	// モデル
	ModelComp *modelComp_ = nullptr;

	// エネルギー
	float energy_ = 0.f;
	// エネルギーの限界量
	VariantItem<float> vMaxEnergy_{ "MaxEnergy", 10.f };
};