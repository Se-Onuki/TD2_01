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

	void Destroy() override;

	void SetIsStun(bool isStun) {
		isStun_ = isStun;
	}
	static void SetOrbComp(Entity *orbComp);
private:
	ModelComp *modelComp_ = nullptr;

	static Entity *pOrb_;

	float energy_ = 0.f;

	bool isStun_ = false;
};

