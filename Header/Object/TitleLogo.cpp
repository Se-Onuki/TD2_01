#include "TitleLogo.h"
#include "../../Utils/SoLib/SoLib.h"
#include "../../Utils/SoLib/SoLib_Lerp.h"

void TitleLogo::Init() {
	state_ = std::make_unique<TitleLogoStateManager>();
	OpLogoEase* st = new OpLogoEase(this);
	state_->SetNextState(st);

	titleLogo_.reset(Sprite::Create(TextureManager::Load("uvChecker.png")));
	titleLogo_->SetPosition({640.0f, 200.0f});
	titleLogo_->SetScale({ 0.0f, 0.0f });
	titleLogo_->SetPivot({ 0.5f, 0.5f });


}

void TitleLogo::Update() {
	if (isChangeSceneCall_ && !preChangeSceneCall_) {
		EpLogoState* st = new EpLogoState(this);
		state_->SetNextState(st);
	}

	state_->Update();
	preChangeSceneCall_ = isChangeSceneCall_;
}

void TitleLogo::Draw() {
	titleLogo_->Draw();
}

#pragma region OpLogo
void OpLogoEase::Init(){

}

void OpLogoEase::Update() {
	titleLogoScale_.x = start_.x + ((end_.x - start_.x) * SoLib::easeOutBack(t_easing_));
	titleLogoScale_.y = start_.y + ((end_.y - start_.y) * SoLib::easeOutBack(t_easing_));

	if (t_easing_ >= 1.0f) {
		t_easing_ = 1.0f;
		DefaultUpLogoEase* st = new DefaultUpLogoEase(parent_);
		stateManager_->SetNextState(st);
	}
	else {
		t_easing_ += 0.1f;
	}
	parent_->SetScale(titleLogoScale_);
}

#pragma endregion

#pragma region DefaultLogo
void DefaultUpLogoEase::Init() {
	Vector2 nowPos = parent_->GetPosition();
	start_ = nowPos;
	end_ = nowPos - Vector2{0.0f, -5.0f};

}

void DefaultUpLogoEase::Update() {
	titleLogoPosition_.x = start_.x + ((end_.x - start_.x) * SoLib::easeInExpo(t_easing_));
	titleLogoPosition_.y = start_.y + ((end_.y - start_.y) * SoLib::easeInExpo(t_easing_));

	if (t_easing_ >= 1.0f) {
		t_easing_ = 1.0f;
		DefaultDownLogoEase* st = new DefaultDownLogoEase(parent_);
		stateManager_->SetNextState(st);
	}
	else {
		t_easing_ += 0.01f;
	}
	parent_->SetPosition(titleLogoPosition_);

}

void DefaultDownLogoEase::Init() {
	Vector2 nowPos = parent_->GetPosition();
	start_ = nowPos;
	end_ = nowPos - Vector2{0.0f, 5.0f};
}

void DefaultDownLogoEase::Update() {
	titleLogoPosition_.x = start_.x + ((end_.x - start_.x) * SoLib::easeInOutBack(t_easing_));
	titleLogoPosition_.y = start_.y + ((end_.y - start_.y) * SoLib::easeInOutBack(t_easing_));

	if (t_easing_ >= 1.0f) {
		t_easing_ = 1.0f;
		DefaultUpLogoEase* st = new DefaultUpLogoEase(parent_);
		stateManager_->SetNextState(st);
	}
	else {
		t_easing_ += 0.01f;
	}
	parent_->SetPosition(titleLogoPosition_);

}

#pragma endregion

#pragma region EpLogo

void EpLogoState::Init() {
	Vector2 nowPos = parent_->GetPosition();
	start_ = nowPos;
	end_ = nowPos - Vector2{0.0f, 300.0f};
	t_colorEasing = 0.0f;
}

void EpLogoState::Update() {
	titleLogoPosition_.x = start_.x + ((end_.x - start_.x) * SoLib::easeInOutBack(t_easing_));
	titleLogoPosition_.y = start_.y + ((end_.y - start_.y) * SoLib::easeInOutBack(t_easing_));
	titleLogoColor_ = SoLib::Lerp(startColor_, endColor_, t_easing_);
		;
	if (t_easing_ >= 1.0f) {
		t_easing_ = 1.0f;
		DefaultUpLogoEase* st = new DefaultUpLogoEase(parent_);
		stateManager_->SetNextState(st);
	}
	else {
		t_easing_ += 0.01f;
	}
	if (t_colorEasing >= 1.0f) {
		t_colorEasing = 1.0f;
	}
	else {
		t_colorEasing += 0.2f;
	}
	parent_->SetPosition(titleLogoPosition_);
	parent_->SetColor(titleLogoColor_);
}
#pragma endregion

void TitleLogoStateManager::Update() {
	if (nextState_) {
		
		state_ = nextState_;
		nextState_ = nullptr;

		state_->SetStateManager(this);

		state_->Init();
	}
	
	state_->Update();
}

