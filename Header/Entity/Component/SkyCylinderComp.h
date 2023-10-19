#pragma once
#include "../Entity.h"
#include "ModelComp.h"
#include "../../../Scene/TitleScene.h"
class SkyCylinderComp : public IComponent{
public:
	using IComponent::IComponent;
	~SkyCylinderComp() = default;

	void Init() override;
	void Update(float deltaTime) override;
	void Reset() override;

private:
	ModelComp* modelComp_ = nullptr;
	
	// シーンチェンジ指令
	bool preChangeSceneCall_ = false;
	
	Vector3 pos_;
	Vector3 start_ = {0.0f, 0.0f, 0.0f};
	Vector3 end_ = {0.0f, 50.0f, 0.0f };
	float t_easing_ = 0.0f;


};

