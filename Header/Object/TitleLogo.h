#pragma once
#include "../../Engine/DirectBase/2D/Sprite.h"
#include <memory>

class TitleLogo;
class TitleLogoStateManager;

// 基底の状態クラス
class LogoEaseState {
public:
	LogoEaseState(TitleLogo* const parent) :
		parent_(parent),
		isEndMove_(false),
		t_easing_(0.0f)
	{}

	virtual ~LogoEaseState() {};

	virtual void SetStateManager(TitleLogoStateManager* stateManager) {
		stateManager_ = stateManager;
	}

	virtual void Init() = 0;
	virtual void Update() = 0;

	virtual bool IsEndMove() { return isEndMove_; }

protected:
	bool isEndMove_ = false;
	TitleLogo* const parent_ = nullptr;
	float t_easing_ = 0.0f;

	TitleLogoStateManager* stateManager_ = nullptr;
};

// 最初の状態
class OpLogoEase : public LogoEaseState {
public:
	using LogoEaseState::LogoEaseState;
	void Init() override;
	void Update() override;
private:

	Vector2 titleLogoScale_ = { 0.0f, 0.0f };
	Vector2 start_ = { 0.0f, 0.0f };
	Vector2 end_ = { 700.0f,200.f };


};

// 通常の状態
class DefaultUpLogoEase : public LogoEaseState {
public:
	using LogoEaseState::LogoEaseState;
	void Init() override;
	void Update() override;
private:
	Vector2 titleLogoPosition_ = { 0.0f, 0.0f };
	Vector2 start_ = { 0.0f, 0.0f };
	Vector2 end_ = { 540.f,540.f };


};

// 通常の状態
class DefaultDownLogoEase : public LogoEaseState {
public:
	using LogoEaseState::LogoEaseState;
	void Init() override;
	void Update() override;
private:
	Vector2 titleLogoPosition_ = { 0.0f, 0.0f };
	Vector2 start_ = { 0.0f, 0.0f };
	Vector2 end_ = { 540.f,540.f };


};
// 終わる時の状態
class EpLogoState : public LogoEaseState {
public:
	using LogoEaseState::LogoEaseState;
	void Init() override;
	void Update() override;
private:
	Vector2 titleLogoPosition_ = { 0.0f, 0.0f };
	Vector2 start_ = { 0.0f, 0.0f };
	Vector2 end_ = { 540.f,540.f };
	Vector4 titleLogoColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector4 startColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector4 endColor_ = { 1.0f, 1.0f, 1.0f, 0.0f };

	float t_colorEasing = 0.0f;
};


class TitleLogoStateManager {
public:
	void SetNextState(LogoEaseState* next) {
		nextState_ = next;
	}

	void Update();



private:
	LogoEaseState* state_;
	LogoEaseState* nextState_;
};

class TitleLogo {

public:
	void Init();
	void Update();
	void Draw();

	Vector2 GetPosition() {
		Vector2 result;
		result.x = titleLogo_->GetTransform().translate.x;
		result.y = titleLogo_->GetTransform().translate.y;
		return result;
	}
	void SetPosition(Vector2 pos) {
		titleLogo_->SetPosition(pos);
	}
	void SetScale(Vector2 scale) {
		titleLogo_->SetScale(scale);
	}
	void SetRotate(float angle) {
		titleLogo_->SetRotate(angle);
	}
	void SetColor(Vector4 color) {
		titleLogo_->SetColor(color);
	}


	void SetIsChangeSceneCall(bool isChange) {
		isChangeSceneCall_ = isChange;
	}
private:
	std::unique_ptr<Sprite> titleLogo_ = nullptr;
	Vector2 titleLogoPos_ = { 640.0f, 200.0f };
	Vector2 titleLogoScale_ = { 0.0f, 0.0f };
	int stateNum_ = -1;
	std::unique_ptr<TitleLogoStateManager> state_ = nullptr;

	bool isChangeSceneCall_ = false;
	bool preChangeSceneCall_ = false;
};


