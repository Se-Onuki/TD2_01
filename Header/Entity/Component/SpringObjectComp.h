#pragma once

#include "../Entity.h"
#include "../../../Engine/DirectBase/Input/Input.h"
#include "../../../Engine/DirectBase/File/VariantItem.h"
#include "../../Object/MapChip.h"
#include "../../../Utils/SoLib/SoLib_Timer.h"
#include "../../../Utils/SoLib/SoLib_Easing.h"

#include <functional>

class PlayerStateManager;
class SpringObjectComp;

class IPlayerState;

template <typename T>
concept IsPlayerState = std::is_base_of<IPlayerState, T>::value;

class IPlayerState {
protected:
	PlayerStateManager *stateManager_ = nullptr;
public:
	IPlayerState(PlayerStateManager *player) :stateManager_(player) {}
	virtual ~IPlayerState() {}
	virtual void Init(float deltaTime) { deltaTime; };
	virtual void Update(float deltaTime) { deltaTime; };

	virtual void Exit(float deltaTime) { deltaTime; };

	virtual void OnCollision(Entity *const other) { other; }
};

class DefaultState : public IPlayerState {
public:
	using IPlayerState::IPlayerState;
	void Init(float deltaTime) override;
	void Update(float deltaTime) override;

	SoLib::RealTimer stateTimer_{};
	Vector3 startModelScale_;
};

class FallingState : public IPlayerState {
public:
	using IPlayerState::IPlayerState;
	void Init(float deltaTime) override;
	void Update(float deltaTime) override;
	void Exit(float deltaTime) override;

	void OnCollision(Entity *const other) override;
};

class JumpingState : public IPlayerState {
public:
	using IPlayerState::IPlayerState;
	void Init(float deltaTime) override;
	void Update(float deltaTime) override;

	void OnCollision(Entity *const other) override;

	SoLib::RealTimer stateTimer_{};
	Vector3 startModelScale_;

	std::function<float(float)> easeFunc = SoLib::easeOutElastic;
};


class SquattingState : public IPlayerState {
public:
	using IPlayerState::IPlayerState;
	void Init(float deltaTime) override;
	void Update(float deltaTime) override;

	SoLib::RealTimer stateTimer_{};
	Vector3 startModelScale_;
};

// Playerクラスの定義
class PlayerStateManager {
private:
	// 現在の状態
	std::unique_ptr<IPlayerState> state_;

	// 予約した状態
	std::unique_ptr<IPlayerState> nextState_ = nullptr;

public:

	SpringObjectComp *const parent_ = nullptr;

public:
	PlayerStateManager(SpringObjectComp *const parent) : state_(std::make_unique<DefaultState>(this)), parent_(parent) {}

	template <IsPlayerState T>
	void ChangeState() {  // 状態を変更するメソッド
		nextState_ = std::make_unique<T>(this);
	}

	template <IsPlayerState T>
	void SetState() {  // 状態を変更するメソッド
		state_ = std::make_unique<T>(this);
	}

	const IPlayerState *const GetState()const { return state_.get(); }

	void Init() {
		state_ = std::make_unique<FallingState>(this);
		nextState_ = nullptr;
	}

	void Update(float deltaTime) {
		if (nextState_) {
			state_ = std::move(nextState_);
			state_->Init(deltaTime);
		}
		state_->Update(deltaTime);
	}

	void OnCollision(Entity *const other) {
		state_->OnCollision(other);
	}
};


class SpringObjectComp : public IComponent {
public:
	using IComponent::IComponent;
	~SpringObjectComp();

	void Init() override;
	void Update(float deltaTime) override;

	void OnCollision(Entity *const other) override;

	VariantItem<float> vJumpString_{ "JumpPower", 15.f };
	VariantItem<float> vMoveString_{ "MovePower", 3.f };

	VariantItem<float> vInvincibleTime_{ "InvincibleTime", 3.f };

	VariantItem<Vector3> vMaxSpeed_{ "MaxSpeed", {3.f,5.0f,0.f} };

	VariantItem<Vector3> vSquatScale_{ "SquatScale", {1.25f,0.5f,1.25f} };
	VariantItem<float> vSquatTime_{ "SquatTime", 0.5f };

	VariantItem<float> vLandingTime_{ "LandingTime", 0.5f };

	VariantItem<float> vHitBox{ "HitBox", 1.5f };

	VariantItem<float> vInputDisableHeight{ "InputDisableHeight", 0.6f };

	VariantItem<float> vJumpAnimTime_{ "JumpAnimTime", 0.25f };

	const auto *const GetManager()const { return state_.get(); }

	/// @brief jsonからの読み込み
	/// @param groupName グループ名
	virtual void ApplyVariables(const char *const groupName);

	/// @brief jsonへの紐づけ
	/// @param groupName グループ名
	virtual void AddVariable(const char *const groupName) const;

private:

	const std::string groupName_ = "Player";

	Input *input_ = nullptr;

	std::unique_ptr<PlayerStateManager> state_ = nullptr;
};