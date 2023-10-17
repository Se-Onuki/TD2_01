#pragma once
#include "../Entity.h"
#include "ModelComp.h"

class OrbComp :public IComponent {
public:
	using IComponent::IComponent;
	~OrbComp() = default;

	void Init() override;
	void Reset() override;

	void SetMaxEnergy(float value) { maxEnergy_ = value; }
	float GetMaxEnergy() const { return maxEnergy_; }

	void SetEnergy(float value) { energy_ = value; }
	void AddEnergy(float value) { energy_ += value; }

	float GetProgress() const { return energy_ / maxEnergy_; }

private:

	ModelComp *modelComp_ = nullptr;

	float energy_ = 0.f;
	float maxEnergy_ = 10.f;
};