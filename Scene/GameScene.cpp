#include "GameScene.h"

#include "../externals/imgui/imgui.h"
#include "../DirectBase/Base/DirectXCommon.h"
#include "../DirectBase/Input/Input.h"
#include "../DirectBase/Base/Audio.h"

#include "TitleScene.h"
#include "GameClearScene.h"
#include "GameOverScene.h"

#include "../Header/Model/ModelManager.h"
#include "../Header/Entity/FollowCamera.h"
#include "../Header/Object/Ground.h"
#include "../Header/Object/SkyCylinder.h"

#include "../Header/Entity/Component/ModelComp.h"
#include "../Header/Entity/Component/PlayerComp.h"

#include "../Header/Entity/Player.h"
#include "../Header/Entity/PlayerBullet.h"
#include "../Header/Entity/Enemy.h"
#include "../Header/Entity/BossBullet.h"

#include "../Header/Entity/Component/Collider.h"

#include "../Header/Collision/Target.h"
#include "../Header/Entity/Component/BossComp.h"


GameScene::GameScene() {
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	collisionManager_ = CollisionManager::GetInstance();
}

GameScene::~GameScene() {

}

void GameScene::OnEnter() {
	targeting_ = Targeting::GetInstance();

	viewProjection_.Init();
	viewProjection_.translation_ = { 0.f,3.f,-15.f };
	viewProjection_.UpdateMatrix();

	auto *const modelManager = ModelManager::GetInstance();

	modelManager->AddModel("playerLeg", Model::LoadObjFile("Model/gunTank/", "playerLeg.obj"));
	modelManager->AddModel("playerWaist", Model::LoadObjFile("Model/gunTank/", "playerWaist.obj"));
	modelManager->AddModel("playerBody", Model::LoadObjFile("Model/gunTank/", "playerBody.obj"));
	modelManager->AddModel("playerHead", Model::LoadObjFile("Model/gunTank/", "playerHead.obj"));

	ModelManager::GetInstance()->AddModel("sphere", Model::LoadObjFile("", "sphere.obj"));

	ModelManager::GetInstance()->AddModel("boss", Model::LoadObjFile("Model/Boss/", "Boss.obj"));
	ModelManager::GetInstance()->AddModel("playerBullet", Model::LoadObjFile("Model/bullet/", "playerBullet.obj"));
	ModelManager::GetInstance()->AddModel("enemyBullet", Model::LoadObjFile("Model/bullet/", "enemyBullet.obj"));
	ModelManager::GetInstance()->AddModel("ground", Model::LoadObjFile("Model/Ground/", "Ground.obj"));
	ModelManager::GetInstance()->AddModel("skyCylinder", Model::LoadObjFile("", "skyCylinder.obj"));

	hudButton_.reset(Sprite::Create(TextureManager::Load("UI/hudButton.png")));
	hudButton_->SetPosition(MiddleCentor);
	hudButton_->SetPivot({ 0.5f,0.5f });
	hudButton_->SetScale(TopRight);

	AddPlayer();

	PopEnemy();

	ground_.reset(new Ground);
	ground_->Init();

	sky_.reset(new SkyCylinder);
	sky_->Init();

	light_.reset(DirectionLight::Create());
}

void GameScene::OnExit() {}

void GameScene::Update() {

	pBulletList_.remove_if([](std::unique_ptr<PlayerBullet> &bullet) {
		if (!bullet->GetActive()) {
			bullet.reset();
			return true;
		}
		return false;
		}
	);

	enemyList_.remove_if([](std::unique_ptr<Enemy> &enemy) {
		if (!enemy->GetActive()) {
			enemy.reset();
			return true;
		}
		return false;
		}
	);

	bossBulletList_.remove_if([](std::unique_ptr<BossBullet> &bBullet) {
		if (!bBullet->GetActive()) {
			bBullet.reset();
			return true;
		}
		return false;
		}
	);

	if (enemyList_.size() < 1u) {
		GameClear();
	}
	if (player_ && !player_->GetActive()) {
		RemovePlayer();
		GameOver();
	}

#pragma region AddCollisionManager
	collisionManager_->clear();

	if (player_) { collisionManager_->push_back(player_.get()); }
	for (auto &pBullet : pBulletList_) {
		collisionManager_->push_back(pBullet.get());
	}
	for (auto &enemy : enemyList_) {
		collisionManager_->push_back(enemy.get());
	}
	for (auto &bBullet : bossBulletList_) {
		collisionManager_->push_back(bBullet.get());
	}

	collisionManager_->ChackAllCollision();

#pragma endregion


#pragma region Target

	targeting_->clear();
	for (auto &enemy : enemyList_) {
		if (!enemy->GetActive())
			continue;
		targeting_->push_back(enemy.get());
	}
	targeting_->Update(viewProjection_);

#pragma endregion
	if (followCamera_) { followCamera_->Update(); }
	if (player_) {
		player_->ImGuiWidget();
		player_->Update();
	}
	for (auto &pBullet : pBulletList_) {
		pBullet->Update();
	}

	for (auto &enemy : enemyList_) {
		enemy->Update();
	}

	for (auto &bBullet : bossBulletList_) {
		bBullet->Update();
	}

	// ground_->Update();

	if (followCamera_) {
		viewProjection_.matView_ = followCamera_->GetViewMatrix();
		viewProjection_.matProjection_ = followCamera_->GetProjectionMatrix();
	}

	if (followCamera_) { followCamera_->ImGuiWidget(); }

	viewProjection_.TransferMatrix();
}

void GameScene::Draw()
{
	DirectXCommon *const dxCommon = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList *const commandList = dxCommon->GetCommandList();

#pragma region 背面スプライト

	Sprite::StartDraw(commandList);

	// スプライトの描画

	Sprite::EndDraw();

#pragma endregion


	dxCommon->CrearDepthBuffer();

#pragma region モデル描画

	Model::StartDraw(commandList);

	light_->SetLight(commandList);

	// モデルの描画
	ground_->Draw(viewProjection_);
	sky_->Draw(viewProjection_);

	if (player_) { player_->Draw(viewProjection_); }

	for (auto &pBullet : pBulletList_) {
		pBullet->Draw(viewProjection_);
	}

	for (auto &enemy : enemyList_) {
		enemy->Draw(viewProjection_);
	}

	for (auto &bBullet : bossBulletList_) {
		bBullet->Draw(viewProjection_);
	}

	Model::EndDraw();

#pragma endregion

#pragma region 前面スプライト

	Sprite::StartDraw(commandList);

	hudButton_->Draw();
	// スプライトの描画
	if (player_) { player_->GetComponent<PlayerComp>()->DrawUI(); }

	for (auto &enemy : enemyList_) {
		enemy->GetComponent<BossComp>()->DrawUI();
	}


	Sprite::EndDraw();

#pragma endregion

}

void GameScene::AddPlayerBullet(PlayerBullet *newBullet) {
	pBulletList_.emplace_back(newBullet);
}

void GameScene::AddEnemy(Enemy *newEnemy) {
	enemyList_.emplace_back(newEnemy);
	enemyList_.back()->transform_.InitResource();
}

void GameScene::AddBossBullet(BossBullet *newBullet) {
	bossBulletList_.emplace_back(newBullet);
	bossBulletList_.back()->transform_.InitResource();
}

void GameScene::AddPlayer() {

	auto *const modelManager = ModelManager::GetInstance();

	Model *const playerLeg = modelManager->GetModel("playerLeg");

	Model *const playerWaist = modelManager->GetModel("playerWaist");
	Model *const playerBody = modelManager->GetModel("playerBody");
	Model *const playerHead = modelManager->GetModel("playerHead");


	player_.reset(new Player);
	player_->Init();
	player_->transform_.translate = Vector3{ 0.f,0.f,-100.f };
	player_->transform_.UpdateMatrix();

	ModelComp *const modelComp = player_->GetComponent<ModelComp>();
	if (modelComp) {
		auto *const waistBone = modelComp->AddBone("waist", playerWaist, Transform{ .translate{ 0.f,0.6f,0.f } });
		modelComp->AddBone("leg", playerLeg, waistBone, {});
		auto *const bodyBone = modelComp->AddBone("body", playerBody, waistBone, Transform{ .translate{ 0.f,0.85f,0.f } });
		modelComp->AddBone("head", playerHead, bodyBone, Transform{ .translate{ 0.f,0.85f,0.f } });
	}

	followCamera_.reset(new FollowCamera);
	followCamera_->Init();
	followCamera_->SetTarget(&player_->transform_);

	PlayerComp *const playerComp = player_->GetComponent<PlayerComp>();
	playerComp->SetViewProjection(followCamera_->GetViewProjection());
	playerComp->SetGameScene(this);
	playerComp->SetFollowCamera(followCamera_.get());

}

void GameScene::RemovePlayer() {
	player_.reset();
	followCamera_.reset();
}

Player *const GameScene::GetPlayer() {
	return player_.get();
}

void GameScene::PopEnemy() {

	Enemy *const enemy = new Enemy;
	enemy->Init();
	enemy->GetComponent<BossComp>()->SetGameScene(this);

	AddEnemy(enemy);
}

void GameScene::GameClear() {
	sceneManager_->ChangeScene(new GameClearScene, 60);
}

void GameScene::GameOver() {
	sceneManager_->ChangeScene(new GameOverScene, 60);

}