#pragma once

#include "../Entity.h"
#include "../../../Engine/DirectBase/Input/Input.h"
#include "../../../Engine/DirectBase/File/VariantItem.h"
#include "../../Object/MapChip.h"

class PlayerStateManager;
class SpringObjectComp;

class IPlayerState {
protected:
	PlayerStateManager *stateManager_ = nullptr;
public:
	IPlayerState(PlayerStateManager *player) :stateManager_(player) {}
	virtual ~IPlayerState() {}
	virtual void Init(float deltaTime) {
		deltaTime;
	};
	virtual void Update(float deltaTime) {
		deltaTime;
	};
};

// 各種具体的な状態クラスの定義
class DefaultState : public IPlayerState {
public:
	using IPlayerState::IPlayerState;
	void Update(float deltaTime) override;
};

class FallingState : public IPlayerState {
public:
	using IPlayerState::IPlayerState;
	void Update(float deltaTime) override;
};

class JumpingState : public IPlayerState {
public:
	using IPlayerState::IPlayerState;
	void Init(float deltaTime) override;
	void Update(float deltaTime) override;
};

template <typename T>
concept IsPlayerState = std::is_base_of<IPlayerState, T>::value;

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

	void Init() {
		state_ = std::make_unique<DefaultState>(this);
		nextState_ = nullptr;
	}

	void Update(float deltaTime) {
		if (nextState_) {
			state_ = std::move(nextState_);
			state_->Init(deltaTime);
		}
		state_->Update(deltaTime);
	}
};


class SpringObjectComp : public IComponent {
public:
	using IComponent::IComponent;
	~SpringObjectComp();

	void Init() override;
	void Update(float deltaTime) override;

	void HitMapChip(const MapChip &mapChip);

	VariantItem<float> vJumpString_{ "JumpPower",15.f };

private:
	Input *input_ = nullptr;

	std::unique_ptr<PlayerStateManager> state_ = nullptr;
};