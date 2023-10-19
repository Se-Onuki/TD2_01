#pragma once
#include "../../Engine/DirectBase/2D/Sprite.h"
#include <memory>

// 前方宣言
class PressSprite;
class PressSpriteStateManager;

// 基底の状態クラス
class PressSpriteEaseState {
public:
	//コンストラクタ
	PressSpriteEaseState(PressSprite* const parent) :
		parent_(parent),
		isEndMove_(false),
		t_easing_(0.0f)
	{}

	// 仮想デストラクタ
	virtual ~PressSpriteEaseState() {};

	// 状態管理クラスの登録
	virtual void SetStateManager(PressSpriteStateManager* stateManager) {
		stateManager_ = stateManager;
	}

	// 初期化処理
	virtual void Init() = 0;

	// 更新処理
	virtual void Update(float deltaTime) = 0;


protected:
	bool isEndMove_ = false;
	PressSprite* const parent_ = nullptr;

	// イージング用の媒介変数
	float t_easing_ = 0.0f;

	// 状態管理クラス
	PressSpriteStateManager* stateManager_ = nullptr;
};

// 最初の状態
class OpPressSpriteEase : public PressSpriteEaseState {
public:
	// 基底クラスのコンストラクタ
	using PressSpriteEaseState::PressSpriteEaseState;

	// 初期化処理
	void Init() override;

	// 更新処理
	void Update(float deltaTime) override;
private:
	// 大きさ
	Vector2 pressSpriteScale_ = { 0.0f, 0.0f };

	// イージング前の大きさ　
	Vector2 start_ = { 0.0f, 0.0f };

	// イージング後の大きさ
	Vector2 end_ = { 300.0f,50.0f };
};

// 通常のアップ状態
class DefaultUpPressSpriteEase : public PressSpriteEaseState {
public:
	// 基底クラスのコンストラクタ
	using PressSpriteEaseState::PressSpriteEaseState;

	// 初期化処理
	void Init() override;

	// 更新処理
	void Update(float deltaTime) override;
private:
	// 位置
	Vector2 pressSpritePosition_ = { 0.0f, 0.0f };

	// イージング前の位置
	Vector2 start_ = { 0.0f, 0.0f };

	// イージング後の位置
	Vector2 end_ = { 540.f,540.f };
};

// 通常のダウン状態
class DefaultDownPressSpriteEase : public PressSpriteEaseState {
public:
	// 基底クラスのコンストラクタ
	using PressSpriteEaseState::PressSpriteEaseState;

	// 初期化処理
	void Init() override;

	// 更新処理
	void Update(float deltaTime) override;
private:
	// 位置
	Vector2 pressSpritePosition_ = { 0.0f, 0.0f };

	// イージング前の位置
	Vector2 start_ = { 0.0f, 0.0f };

	// イージング後の位置
	Vector2 end_ = { 540.f,540.f };
};

// 終わる時の状態
class EpPressSpriteState : public PressSpriteEaseState {
public:
	// 基底クラスのコンストラクタ
	using PressSpriteEaseState::PressSpriteEaseState;

	// 初期化処理
	void Init() override;

	// 更新処理
	void Update(float deltaTime) override;
private:
	// 位置
	Vector2 pressSpritePosition_ = { 0.0f, 0.0f };

	// イージング前の位置
	Vector2 start_ = { 0.0f, 0.0f };

	// イージング後の位置
	Vector2 end_ = { 540.f,540.f };

	// 色(RGBA)
	Vector4 pressSpriteColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// イージング前の色
	Vector4 startColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// イージング後の色
	Vector4 endColor_ = { 1.0f, 1.0f, 1.0f, 0.0f };

	// 色イージング用の媒介変数
	float t_colorEasing = 0.0f;
};

// タイトルロゴの状態管理クラス
class PressSpriteStateManager {
public:
	// 次のシーン予約
	void SetNextState(PressSpriteEaseState* next) {
		nextState_ = next;
	}

	// 更新処理
	void Update(float deltaTime);
private:
	// 今の状態
	PressSpriteEaseState* state_;

	// 次の状態の予約
	PressSpriteEaseState* nextState_ = nullptr;
};

// タイトルロゴ
class PressSprite {
public:
	// 初期化処理
	void Init();

	// 更新処理
	void Update(float deltaTime);

	// 描画処理
	void Draw();

	// 位置の取得
	Vector2 GetPosition() {
		Vector2 result;
		result.x = pressSprite->GetTransform().translate.x;
		result.y = pressSprite->GetTransform().translate.y;
		return result;
	}
	// 位置の設定
	void SetPosition(Vector2 pos) {
		pressSprite->SetPosition(pos);
	}
	// 大きさの設定
	void SetScale(Vector2 scale) {
		pressSprite->SetScale(scale);
	}
	// 角度の設定
	void SetRotate(float angle) {
		pressSprite->SetRotate(angle);
	}
	// 色の設定
	void SetColor(Vector4 color) {
		pressSprite->SetColor(color);
	}
	// シーンチェンジ指令
	void SetIsChangeSceneCall(bool isChange) {
		isChangeSceneCall_ = isChange;
	}
private:
	// タイトルロゴのスプライト
	std::unique_ptr<Sprite> pressSprite = nullptr;

	// 位置
	Vector2 pressSpritePos_ = { 640.0f, 600.0f };

	// 大きさ
	Vector2 pressSpriteScale_ = { 0.0f, 0.0f };
	int stateNum_ = -1;
	// 状態
	std::unique_ptr<PressSpriteStateManager> state_ = nullptr;

	// シーンチェンジ指令
	bool isChangeSceneCall_ = false;
	bool preChangeSceneCall_ = false;
};


