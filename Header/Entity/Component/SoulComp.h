#pragma once
#include "../Entity.h"
#include "ModelComp.h"

class SoulComp : public IComponent {
public:
	using IComponent::IComponent;
	~SoulComp() = default;

	void Init() override;
	void Update(float deltaTime) override;
	void Reset() override;

	void SetOrbComp(Entity* orbComp);
private:
	ModelComp* modelComp_ = nullptr;

	Entity* pOrb_ = nullptr;

};

