#include "PressSprite.h"
#include "../../Utils/SoLib/SoLib.h"
#include "../../Utils/SoLib/SoLib_Lerp.h"

void PressSprite::Init() {
	// 状態管理クラスを生成
	//state_.release();
	state_ = std::make_unique<PressSpriteStateManager>();
	
	// 最初の状態を登録
	OpPressSpriteEase* st = new OpPressSpriteEase(this);
	state_->SetNextState(st);

	// タイトルロゴのスプライトを生成、いろいろ設定
	pressSprite.reset(Sprite::Create(TextureManager::Load("uvChecker.png")));
	pressSprite->SetPosition({640.0f, 400.0f });
	pressSprite->SetScale({ 0.0f, 0.0f });
	pressSprite->SetPivot({ 0.5f, 0.5f });


}

void PressSprite::Update(float deltaTime) {
	// もしシーンチェンジ指令がきたら
	if (isChangeSceneCall_ && !preChangeSceneCall_) {
		// 終了時の状態に切り替える
		EpPressSpriteState* st = new EpPressSpriteState(this);
		state_->SetNextState(st);
	}

	// 状態の更新処理
	state_->Update(deltaTime);

	// 前フレーム時のシーンチェンジ指令を保存
	preChangeSceneCall_ = isChangeSceneCall_;
}

void PressSprite::Draw() {
	// タイトルロゴのスプライト描画
	pressSprite->Draw();
}

#pragma region OpLogo
void OpPressSpriteEase::Init(){
	// 大きさ
	pressSpriteScale_ = { 0.0f, 0.0f };

	// イージング前の大きさ　
	start_ = { 0.0f, 0.0f };

	// イージング後の大きさ
	end_ = { 300.0f,50.0f };
}

void OpPressSpriteEase::Update(float deltaTime) {
	// 大きさのイージング
	pressSpriteScale_.x = start_.x + ((end_.x - start_.x) * SoLib::easeOutBack(t_easing_));
	pressSpriteScale_.y = start_.y + ((end_.y - start_.y) * SoLib::easeOutBack(t_easing_));

	// イージングの媒介変数の処理
	if (t_easing_ >= 1.0f) {
		t_easing_ = 1.0f;
		DefaultUpPressSpriteEase* st = new DefaultUpPressSpriteEase(parent_);
		stateManager_->SetNextState(st);
	}
	else {
		t_easing_ += 0.8f * deltaTime;
	}

	// 大きさを設定
	parent_->SetScale(pressSpriteScale_);
}

#pragma endregion

#pragma region DefaultLogo
void DefaultUpPressSpriteEase::Init() {
	Vector2 nowPos = parent_->GetPosition();
	start_ = nowPos;
	end_ = nowPos - Vector2{0.0f, -50.0f};

}

void DefaultUpPressSpriteEase::Update(float deltaTime) {
	// 位置のイージング
	pressSpritePosition_.x = start_.x + ((end_.x - start_.x) * SoLib::easeOutBack(t_easing_));
	pressSpritePosition_.y = start_.y + ((end_.y - start_.y) * SoLib::easeOutBack(t_easing_));
	
	// イージングの媒介変数の処理
	if (t_easing_ >= 1.0f) {
		t_easing_ = 1.0f;
		DefaultDownPressSpriteEase* st = new DefaultDownPressSpriteEase(parent_);
		stateManager_->SetNextState(st);
	}
	else {
		t_easing_ += 1.5f * deltaTime;
	}

	// 位置を設定
	parent_->SetPosition(pressSpritePosition_);
}

void DefaultDownPressSpriteEase::Init() {
	Vector2 nowPos = parent_->GetPosition();
	start_ = nowPos;
	end_ = nowPos - Vector2{0.0f, 50.0f};
}

void DefaultDownPressSpriteEase::Update(float deltaTime) {
	// 位置のイージング
	pressSpritePosition_.x = start_.x + ((end_.x - start_.x) * SoLib::easeOutBack(t_easing_));
	pressSpritePosition_.y = start_.y + ((end_.y - start_.y) * SoLib::easeOutBack(t_easing_));
	
	// イージングの媒介変数の処理
	if (t_easing_ >= 1.0f) {
		t_easing_ = 1.0f;
		DefaultUpPressSpriteEase* st = new DefaultUpPressSpriteEase(parent_);
		stateManager_->SetNextState(st);
	}
	else {
		t_easing_ += 1.5f * deltaTime;
	}

	// 位置を設定
	parent_->SetPosition(pressSpritePosition_);
}

#pragma endregion

#pragma region EpLogo

void EpPressSpriteState::Init() {
	Vector2 nowPos = parent_->GetPosition();
	start_ = nowPos;
	end_ = nowPos - Vector2{0.0f, 300.0f};
	t_colorEasing = 0.0f;
}

void EpPressSpriteState::Update(float deltaTime) {
	// 位置のイージング
	pressSpritePosition_.x = start_.x + ((end_.x - start_.x) * SoLib::easeInOutBack(t_easing_));
	pressSpritePosition_.y = start_.y + ((end_.y - start_.y) * SoLib::easeInOutBack(t_easing_));
	
	// 色のイージング
	pressSpriteColor_ = SoLib::Lerp(startColor_, endColor_, t_colorEasing);
	
	// イージングの媒介変数の処理
	if (t_easing_ >= 1.0f) {
		t_easing_ = 1.0f;
	}
	else {
		t_easing_ += 1.0f * deltaTime;
	}

	// 色イージングの媒介変数の処理
	if (t_colorEasing >= 1.0f) {
		t_colorEasing = 1.0f;
	}
	else {
		t_colorEasing += 1.5f * deltaTime;
	}

	// 位置、色を設定
	parent_->SetPosition(pressSpritePosition_);
	parent_->SetColor(pressSpriteColor_);
}
#pragma endregion

void PressSpriteStateManager::Update(float deltaTime) {
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
	state_->Update(deltaTime);
}

