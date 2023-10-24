#pragma once
#include "../../Engine/DirectBase/2D/Sprite.h"
#include "../../Engine/DirectBase/File/VariantItem.h"
#include <memory>

// 敵の残りの数
class RemainEnemy {
public:
	// 初期化処理
	void Init();
	// 更新処理
	void Update();
	// 描画処理
	void Draw();

	// 現在の敵の数を設定
	void SetNumber(int num) { enemyNumber_ = num; }
	
	// 敵の最大数を設定
	void SetMaxNumber(int num) { maxEnemyNumber_ = num; }

	// 位置
	VariantItem<Vector2> vPos_{"Position", { 640.0f, 360.0f }};
	
	// 大きさ(0.0f ~ 1.0f)
	VariantItem<float> vScale_{ "Scale", 0.5f};
	
	// 数字の間隔( "/" は変わりません )
	VariantItem<float> vNumSpacing_ {"NumberSpacing", 100.0f};

	/// @brief jsonからの読み込み
	/// @param groupName グループ名
	virtual void ApplyVariables(const char* const groupName);

	/// @brief jsonへの紐づけ
	/// @param groupName グループ名
	virtual void AddVariable(const char* const groupName) const;

private:
	const std::string groupName_ = "RemainEnemy";

	// 数字のスプライト
	std::unique_ptr<Sprite> numberSprite_[4];

	// "/" のスプライト
	std::unique_ptr<Sprite> slashSprite_;

	// 数字一つの横幅
	float numWidth;
	// 数字一つの縦幅
	float numHeight;

	// 数字(1桁ずつ)
	int num_[4];

	// 現在の敵の数
	int enemyNumber_;

	// 敵の最大数
	int maxEnemyNumber_;
};

