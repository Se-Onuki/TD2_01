#pragma once

#include "../../Engine/DirectBase/Model/ModelManager.h"
#include "../../Engine/DirectBase/Render/Camera.h"
#include "../../Utils/Math/Transform.h"

class SkyCylinder {
public:
	//using IComponent::IComponent;
	SkyCylinder();
	~SkyCylinder();

	void Init(const std::string& fileName);
	void Update(float deltaTime);
	void Draw(const Camera<Render::CameraType::Projecction>& camera);
	void Reset();
	void SetChangeSceneCall(bool changeSceneCall) {
		changeSceneCall_ = changeSceneCall;
	}
private:
	// シーンチェンジ指令
	bool changeSceneCall_ = false;
	bool preChangeSceneCall_ = false;

	Transform worldTransform_;
	Model* model_ = nullptr;
	Vector3 pos_;
	Vector3 start_ = {0.0f, 0.0f, 0.0f};
	Vector3 end_ = {0.0f, 50.0f, 0.0f };
	float t_easing_ = 0.0f;


};

