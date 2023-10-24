#pragma once
#include "../Entity.h"
#include "../../../Engine/DirectBase/Model/Model.h"
#include <iostream>
#include <memory>
#include <concepts>
#include "../../../Engine/DirectBase/File/VariantItem.h"
#include "../../../Engine/DirectBase/2D/Sprite.h"

class EnemyComp;
namespace EnemyState {
	class IState {
	public:
		IState(EnemyComp *sauce) :enemy_(sauce) {}

		EnemyComp *const enemy_ = nullptr;

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
	template <typename T>
	concept IsBaseIState = std::derived_from<T, IState>;

	class StapUpState :public IState {
	public:
		using IState::IState;
		void Init(float deltaTime) override;

		void Update(float deltaTime) override;

		void Exit(float deltaTime) override;
	};

	class IdleState : public IState {
	public:
		using IState::IState;
		void Init(float deltaTime) override;

		void Update(float deltaTime) override;

		void Exit(float deltaTime) override;
	};

	class ApproachState : public IState {
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
	void Draw2D() const override;

	void OnCollision(Entity *const other) override;

	void Destroy() override;

	/// @brief スタン中の敵全破壊
	void BreakAll();

	static void SetEnemyList(std::list<std::unique_ptr<Entity>> *const enemys) { sEnemys_ = enemys; }

	static void StaticUpdate(float deltaTime);
	static void StaticInit();

	template <EnemyState::IsBaseIState T>
	void SetState() {
		if (nextState_ == nullptr) {
			nextState_ = std::make_unique<T>(this);
		}
	}

	bool GetIsStan() const { return isStan_; }
	void SetIsStan(const bool stan) { isStan_ = stan; }

	float GetStanTime() const { return sStanTime_; }

	void StartStan();

	static int GetStunSum() {
		return stunCount_;
	}
	static int GetEnemySum() {
		return int(sEnemys_->size());

	}

private:

	void SetStanTime(float stanTime) { sStanTime_ = stanTime; }


public:

	// デフォルトのスタン時間
	VariantItem<float> vDefaultStanTime_{ "StanTime", 10.f };


private:

	bool isStan_ = false;
	static float sStanTime_;

	static std::list<std::unique_ptr<Entity>> *sEnemys_;

	std::unique_ptr<EnemyState::IState> currentState_;
	std::unique_ptr<EnemyState::IState> nextState_;

	uint32_t oneStunTex;
	uint32_t twoStunTex;
	std::unique_ptr<Sprite> stunSprite_ = nullptr;

	static int stunCount_;
};
