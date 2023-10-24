#include "RemainEnemy.h"
#include "../../Engine/DirectBase/File/GlobalVariables.h"

void RemainEnemy::Init() {
	// グローバル変数の値を設定
	ApplyVariables(groupName_.c_str());
	AddVariable(groupName_.c_str());
	const float& scale = vScale_;

	// それぞれスプライトの初期化処理
	for (int i = 0; i < 4; i++) {
		numberSprite_[i].reset(Sprite::Create(TextureManager::Load("nums.png")));
		numberSprite_[i]->SetScale({ numWidth * scale, numHeight * scale });
		numberSprite_[i]->SetPivot({ 0.5f, 0.5f });
	}

	// "/" のスプライトの初期化処理
	slashSprite_.reset(Sprite::Create(TextureManager::Load("slash.png")));
	slashSprite_->SetScale({ numWidth * scale, numHeight * scale });
	slashSprite_->SetPivot({ 0.5f, 0.5f });

	// 数字一つの横幅と縦幅を設定
	numWidth = numberSprite_[0]->GetUV().second.x / 10.0f;
	numHeight = numberSprite_[0]->GetUV().second.y;
}

void RemainEnemy::Update() {
	// 値を適用させる
	ApplyVariables(groupName_.c_str());

	// グローバル変数の値
	const Vector2& pos = vPos_;
	const float& scale = vScale_;
	const float& numSpacing = vNumSpacing_;

	// 一桁ずつ数字を抜き取る
	num_[1] = enemyNumber_ / 10;
	enemyNumber_ = enemyNumber_ % 10;
	num_[0] = enemyNumber_ / 1;
	num_[2] = maxEnemyNumber_ / 10;
	enemyNumber_ = maxEnemyNumber_ % 10;
	num_[3] = maxEnemyNumber_ / 1;

	// 抜き出した数字をもとにテクスチャの切り取る位置を決める
	for (int i = 0; i < 4; i++) {
		numberSprite_[i]->SetTexOrigin({ numWidth * num_[i], 0.0f });
		numberSprite_[i]->SetTexDiff({ numWidth , numHeight });
		numberSprite_[i]->SetScale({ numWidth * scale, numHeight * scale });
	}	slashSprite_->SetScale({ numWidth * scale, numHeight * scale });

	// それぞれの数字の位置を設定
	numberSprite_[0]->SetPosition({ pos.x - 30.0f, pos.y});
	
	numberSprite_[1]->SetPosition(
		{ numberSprite_[0]->GetTransform().translate.x - numSpacing, pos.y});

	slashSprite_->SetPosition({ pos.x, pos.y });

	numberSprite_[2]->SetPosition({ pos.x + 30.0f, pos.y });
	
	numberSprite_[3]->SetPosition(
		{ numberSprite_[2]->GetTransform().translate.x + numSpacing, pos.y });

}

void RemainEnemy::Draw() {
	numberSprite_[0]->Draw();
	numberSprite_[1]->Draw();
	slashSprite_->Draw();
	numberSprite_[2]->Draw();
	numberSprite_[3]->Draw();
}

void RemainEnemy::ApplyVariables(const char* const groupName) {
	const GlobalVariables* const gVariable = GlobalVariables::GetInstance();
	const auto& cGroup = gVariable->GetGroup(groupName);

	cGroup >> vPos_;
	cGroup >> vScale_;
	cGroup >> vNumSpacing_;
}

void RemainEnemy::AddVariable(const char* const groupName) const {
	GlobalVariables* const gVariable = GlobalVariables::GetInstance();
	auto& group = gVariable->GetGroup(groupName);

	group << vPos_;
	group << vScale_;
	group << vNumSpacing_;
}