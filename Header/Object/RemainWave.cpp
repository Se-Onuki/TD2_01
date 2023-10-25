#include "RemainWave.h"
#include "../../Engine/DirectBase/File/GlobalVariables.h"

void RemainWave::Init() {
	// グローバル変数の値を設定
	ApplyVariables(groupName_.c_str());
	AddVariable(groupName_.c_str());
	const float& scale = vScale_;

	const float alpha = 1.0f;

	// それぞれスプライトの初期化処理
	for (int i = 0; i < 4; i++) {
		numberSprite_[i].reset(Sprite::Create(TextureManager::Load("nums.png")));
		numberSprite_[i]->SetScale({ numWidth * scale, numHeight * scale });
		numberSprite_[i]->SetPivot({ 0.5f, 0.5f });
		numberSprite_[i]->SetColor({ 1.0f, 1.0f, 1.0f, alpha });
	}

	numberSprite_[2]->SetColor({ 1.0f, 0.4f, 0.4f, alpha });
	numberSprite_[3]->SetColor({ 1.0f, 0.4f, 0.4f, alpha });

	// "/" のスプライトの初期化処理
	slashSprite_.reset(Sprite::Create(TextureManager::Load("slash.png")));
	slashSprite_->SetScale({ numWidth * scale, numHeight * scale });
	slashSprite_->SetPivot({ 0.5f, 0.5f });
	slashSprite_->SetColor({ 1.0f, 1.0f, 1.0f, alpha });


	enemySprite_.reset(Sprite::Create(TextureManager::Load("Wave.png")));
	enemySprite_->SetScale({ 300.0f * scale,128.0f * scale });
	enemySprite_->SetPivot({ 0.5f, 0.5f });
	enemySprite_->SetColor({ 1.0f, 1.0f, 1.0f, alpha });


	// 数字一つの横幅と縦幅を設定
	numWidth = numberSprite_[0]->GetUV().second.x / 10.0f;
	numHeight = numberSprite_[0]->GetUV().second.y;

}

void RemainWave::Update() {
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
	numberSprite_[0]->SetPosition({ pos.x - 30.0f, pos.y });

	numberSprite_[1]->SetPosition(
		{ numberSprite_[0]->GetTransform().translate.x - numSpacing, pos.y });

	slashSprite_->SetPosition({ pos.x, pos.y });

	numberSprite_[2]->SetPosition({ pos.x + 30.0f, pos.y });

	numberSprite_[3]->SetPosition(
		{ numberSprite_[2]->GetTransform().translate.x + numSpacing, pos.y });
	enemySprite_->SetPosition({ numberSprite_[1]->GetTransform().translate.x - (220.0f * scale), pos.y });


}

void RemainWave::Draw() {
	numberSprite_[0]->Draw();
	numberSprite_[1]->Draw();
	slashSprite_->Draw();
	numberSprite_[2]->Draw();
	numberSprite_[3]->Draw();
	enemySprite_->Draw();
}

void RemainWave::ApplyVariables(const char* const groupName) {
	const GlobalVariables* const gVariable = GlobalVariables::GetInstance();
	const auto& cGroup = gVariable->GetGroup(groupName);

	cGroup >> vPos_;
	cGroup >> vScale_;
	cGroup >> vNumSpacing_;
}

void RemainWave::AddVariable(const char* const groupName) const {
	GlobalVariables* const gVariable = GlobalVariables::GetInstance();
	auto& group = gVariable->GetGroup(groupName);

	group << vPos_;
	group << vScale_;
	group << vNumSpacing_;
}
