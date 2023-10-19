#pragma once
#include "../Entity.h"

#include "../../../Utils/Math/Vector3.h"
#include "../../../Utils/SoLib/SoLib_Traits.h"

class Rigidbody : public IComponent {
public:
	using IComponent::IComponent;
	~Rigidbody() = default;

	void Init();
	void Update(float deltaTime) override;

	/// @brief 瞬間的な加速を行う
	/// @param vec 加速度
	void ApplyInstantForce(const Vector3 &vec) { acceleration_ += vec; }

	/// @brief 継続的な加速
	/// @param vec 加速度
	/// @param deltaTime 時間差分
	void ApplyContinuousForce(const Vector3 &vec, float deltaTime) { acceleration_ += vec * deltaTime * object_->timeScale_; }

	void SetAcceleration(const Vector3 &vec) { acceleration_ = vec; }
	const Vector3 &GetAcceleration() const { return acceleration_; }

	void SetVelocity(const Vector3 &vec) { velocity_ = vec; }

	const Vector3 &GetVelocity() const { return velocity_; }

	bool GetIsGround() const { return isGround_; }
	void SetIsGround(const bool isGround) { isGround_ = isGround; }

	const Vector3 &GetMaxSpeed() const { return maxSpeed_; }
	void SetMaxSpeed(const Vector3 &hoge) { maxSpeed_ = hoge; }

	bool hasCollider_ = false;
	float radius_ = 1.f;

private:
	Vector3 velocity_{};
	Vector3 acceleration_{};

	Vector3 maxSpeed_{ -1.f,-1.f,-1.f };	// 負数である場合は無効化

	bool isGround_ = false;

	static const float deltaTime_;
};

// 空気中での粘性摩擦の計算関数
inline Vector3 CalcFriction(const Vector3 &velocity, float coefficient) {
	// 速度ベクトルに対して逆向きの摩擦力ベクトルを計算
	Vector3 frictionForce = velocity.Nomalize();
	frictionForce = frictionForce * (-0.5f * coefficient * velocity.LengthSQ());

	return frictionForce;
}