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

	void OnCollision(Entity *const other) override;

	void Destroy() override;

	/// @brief スタン中の敵全破壊
	void SelfBreak();

	bool GetIsStan() const { return isStan_; }

	static void SetEnemyList(std::list<std::unique_ptr<Entity>> *const enemys) { sEnemys_ = enemys; }

	static void StaticUpdate(float deltaTime);

private:

	void SetStanTime(float stanTime) { sStanTime_ = stanTime; }

private:

	bool isStan_ = false;
	static float sStanTime_;

	Model *defaultModel_ = nullptr;

	static std::list<std::unique_ptr<Entity>> *sEnemys_;

};
