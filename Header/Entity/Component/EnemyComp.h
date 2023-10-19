#pragma once
#include "../Entity.h"
#include "../../../Engine/DirectBase/Model/Model.h"
#include <iostream>
#include <memory>
#include "../../../Engine/DirectBase/File/VariantItem.h"

class EnemyComp;
namespace EnemyState {
	class IState {
	public:
		IState(EnemyComp *sauce) :sauce_(sauce) {}

		EnemyComp *const sauce_ = nullptr;

		virtual void Init(float deltaTime) {
			deltaTime;
		};
		virtual void Update(float deltaTime) {
			deltaTime;
		};

		virtual void Exit(float deltaTime) {
			deltaTime;
		};

		virtual void OnCollision(Entity *const other) { other; }
	};

	class IdleState : public IState {
	public:
		using IState::IState;
		void Init(float deltaTime) override;

		void Update(float deltaTime) override;

		void Exit(float deltaTime) override;
	};

	class AttackState : public IState {
	public:
		using IState::IState;
		void Init(float deltaTime) override;

		void Update(float deltaTime) override;

		void Exit(float deltaTime) override;
	};

	class StunState : public IState {
	public:
		using IState::IState;
		void Init(float deltaTime) override;

		void Update(float deltaTime) override;

		void Exit(float deltaTime) override;
		float remainingTime_;
	};
}
class EnemyComp :public IComponent {
public:
	using IComponent::IComponent;
	~EnemyComp() = default;

	void Init() override;
	void Update(float deltaTime) override;
	void Draw(const Camera<Render::CameraType::Projecction> &camera) const override;

	void OnCollision(Entity *const other) override;

	void Destroy() override;

	/// @brief スタン中の敵全破壊
	void BreakAll();

	bool GetIsStan() const { return isStan_; }

	static void SetEnemyList(std::list<std::unique_ptr<Entity>> *const enemys) { sEnemys_ = enemys; }

	static void StaticUpdate(float deltaTime);


	void SetState(EnemyState::IState *const newState) {
		if (nextState_ == nullptr) {
			nextState_.reset(newState);
		}
	}

private:

	void SetStanTime(float stanTime) { sStanTime_ = stanTime; }


public:

	// デフォルトのスタン時間
	VariantItem<float> vDefaultStanTime_{ "StanTime", 10.f };

private:

	bool isStan_ = false;
	static float sStanTime_;

	Model *defaultModel_ = nullptr;

	static std::list<std::unique_ptr<Entity>> *sEnemys_;

	std::unique_ptr<EnemyState::IState> currentState_;
	std::unique_ptr<EnemyState::IState> nextState_;

};
