#include "TitleLogo.h"
#include "../../Utils/SoLib/SoLib.h"
#include "../../Utils/SoLib/SoLib_Lerp.h"

void TitleLogo::Init() {
	// 状態管理クラスを生成
	//state_.release();
	state_ = std::make_unique<TitleLogoStateManager>();
	
	// 最初の状態を登録
	OpLogoEase* st = new OpLogoEase(this);
	state_->SetNextState(st);

	// タイトルロゴのスプライトを生成、いろいろ設定
	titleLogo_.reset(Sprite::Create(TextureManager::Load("uvChecker.png")));
	titleLogo_->SetPosition({640.0f, 200.0f});
	titleLogo_->SetScale({ 0.0f, 0.0f });
	titleLogo_->SetPivot({ 0.5f, 0.5f });


}

void TitleLogo::Update() {
	// もしシーンチェンジ指令がきたら
	if (isChangeSceneCall_ && !preChangeSceneCall_) {
		// 終了時の状態に切り替える
		EpLogoState* st = new EpLogoState(this);
		state_->SetNextState(st);
	}

	// 状態の更新処理
	state_->Update();

	// 前フレーム時のシーンチェンジ指令を保存
	preChangeSceneCall_ = isChangeSceneCall_;
}

void TitleLogo::Draw() {
	// タイトルロゴのスプライト描画
	titleLogo_->Draw();
}

#pragma region OpLogo
void OpLogoEase::Init(){
	// 大きさ
	titleLogoScale_ = { 0.0f, 0.0f };

	// イージング前の大きさ　
	start_ = { 0.0f, 0.0f };

	// イージング後の大きさ
	end_ = { 700.0f,200.f };
}

void OpLogoEase::Update() {
	// 大きさのイージング
	titleLogoScale_.x = start_.x + ((end_.x - start_.x) * SoLib::easeOutBack(t_easing_));
	titleLogoScale_.y = start_.y + ((end_.y - start_.y) * SoLib::easeOutBack(t_easing_));

	// イージングの媒介変数の処理
	if (t_easing_ >= 1.0f) {
		t_easing_ = 1.0f;
		DefaultUpLogoEase* st = new DefaultUpLogoEase(parent_);
		stateManager_->SetNextState(st);
	}
	else {
		t_easing_ += 0.1f;
	}

	// 大きさを設定
	parent_->SetScale(titleLogoScale_);
}

#pragma endregion

#pragma region DefaultLogo
void DefaultUpLogoEase::Init() {
	Vector2 nowPos = parent_->GetPosition();
	start_ = nowPos;
	end_ = nowPos - Vector2{0.0f, -10.0f};

}

void DefaultUpLogoEase::Update() {
	// 位置のイージング
	titleLogoPosition_.x = start_.x + ((end_.x - start_.x) * SoLib::easeInExpo(t_easing_));
	titleLogoPosition_.y = start_.y + ((end_.y - start_.y) * SoLib::easeInExpo(t_easing_));
	
	// イージングの媒介変数の処理
	if (t_easing_ >= 1.0f) {
		t_easing_ = 1.0f;
		DefaultDownLogoEase* st = new DefaultDownLogoEase(parent_);
		stateManager_->SetNextState(st);
	}
	else {
		t_easing_ += 0.01f;
	}

	// 位置を設定
	parent_->SetPosition(titleLogoPosition_);
}

void DefaultDownLogoEase::Init() {
	Vector2 nowPos = parent_->GetPosition();
	start_ = nowPos;
	end_ = nowPos - Vector2{0.0f, 5.0f};
}

void DefaultDownLogoEase::Update() {
	// 位置のイージング
	titleLogoPosition_.x = start_.x + ((end_.x - start_.x) * SoLib::easeInOutBack(t_easing_));
	titleLogoPosition_.y = start_.y + ((end_.y - start_.y) * SoLib::easeInOutBack(t_easing_));
	
	// イージングの媒介変数の処理
	if (t_easing_ >= 1.0f) {
		t_easing_ = 1.0f;
		DefaultUpLogoEase* st = new DefaultUpLogoEase(parent_);
		stateManager_->SetNextState(st);
	}
	else {
		t_easing_ += 0.01f;
	}

	// 位置を設定
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
	// 位置のイージング
	titleLogoPosition_.x = start_.x + ((end_.x - start_.x) * SoLib::easeInOutBack(t_easing_));
	titleLogoPosition_.y = start_.y + ((end_.y - start_.y) * SoLib::easeInOutBack(t_easing_));
	
	// 色のイージング
	titleLogoColor_ = SoLib::Lerp(startColor_, endColor_, t_colorEasing);
	
	// イージングの媒介変数の処理
	if (t_easing_ >= 1.0f) {
		t_easing_ = 1.0f;
		DefaultUpLogoEase* st = new DefaultUpLogoEase(parent_);
		stateManager_->SetNextState(st);
	}
	else {
		t_easing_ += 0.01f;
	}

	// 色イージングの媒介変数の処理
	if (t_colorEasing >= 1.0f) {
		t_colorEasing = 1.0f;
	}
	else {
		t_colorEasing += 0.2f;
	}

	// 位置、色を設定
	parent_->SetPosition(titleLogoPosition_);
	parent_->SetColor(titleLogoColor_);
}
#pragma endregion

void TitleLogoStateManager::Update() {
	// もし次の状態が予約されていたら
	if (nextState_) {
		// 予約されている状態を設定
		state_ = nextState_;
		// 予約枠はあける
		nextState_ = nullptr;

		// 状態管理クラスを設定
		state_->SetStateManager(this);

		// 状態の初期化処理
		state_->Init();
	}
	
	// 状態の更新処理
	state_->Update();
}

