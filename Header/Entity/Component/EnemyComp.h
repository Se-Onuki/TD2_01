#pragma once
#include "../Entity.h"
#include "../../../Engine/DirectBase/Model/Model.h"

class EnemyComp :public IComponent {
public:
	using IComponent::IComponent;
	~EnemyComp() = default;

	void Init() override;
	void Update(float deltaTime) override;
	void Draw(const Camera<Render::CameraType::Projecction> &camera) const override;

	void Destroy() override;

private:

	Model *defaultModel_ = nullptr;

};
