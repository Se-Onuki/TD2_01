#pragma once
#include "../Object.h"
#include "../../File/GlobalVariables.h"

class BossComp : public IComponent {
public:
	using IComponent::IComponent;
	~BossComp() override = default;

	void Init() override;

	void Update() override;


	/// @brief jsonからの読み込み
	/// @param groupName グループ名
	virtual void ApplyVariables(const char *const groupName) override;

	/// @brief jsonへの紐づけ
	/// @param groupName グループ名
	virtual void AddVariable(const char *const groupName) override;


private:

	static const char *const groupName_;

	VariantItem<float> vHealth_{ "health" ,4.f };
	float vMaxHealth_ = 100.f;

};