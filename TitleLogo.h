#pragma once
#include "./Engine/DirectBase/2D/Sprite.h"
#include <memory>


// タイトルロゴ
class TitleLogo {
public: // メンバ関数
	/// <summary>
	/// 初期化処理
	/// </summary>
	void OnEnter();

	/// <summary>
	/// 毎フレーム更新処理
	/// </summary>
	void Update();
	
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private: // メンバ変数
	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;
};