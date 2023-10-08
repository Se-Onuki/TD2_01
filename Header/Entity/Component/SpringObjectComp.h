#pragma once

#include "../Entity.h"
#include "../../../Engine/DirectBase/Input/Input.h"

class SpringObjectComp : public IComponent {
public:
	using IComponent::IComponent;
	~SpringObjectComp();

	void Init() override;
	void Update(float deltaTime) override;

	void Jump();

private:
	Input *input_ = nullptr;
};