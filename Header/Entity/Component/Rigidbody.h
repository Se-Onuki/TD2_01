#pragma once
#include "../Entity.h"

#include "../../../Utils/Math/Vector3.h"

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

	bool hasCollider_ = false;
	float radius_ = 1.f;

private:
	Vector3 velocity_{};
	Vector3 acceleration_{};

	bool isGround_ = false;

	static const float deltaTime_;
};