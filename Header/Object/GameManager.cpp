#include "Gamemanager.h"

#include <fstream>


#include "../../Engine/DirectBase/Model/ModelManager.h"

#include "../Entity/Component/ModelComp.h"
#include "../Entity/Component/SpringObjectComp.h"
#include "../Entity/Component/Rigidbody.h"
#include "../Entity/Component/EnemyComp.h"
#include "../Entity/Component/OrbComp.h"
#include "../Entity/Component/OrbGaugeComp.h"
#include "../Entity/Component/SoulComp.h"
#include "../../Utils/SoLib/SoLib.h"

void GameManager::Init() {

	collisionManager_ = CollisionManager::GetInstance();

#pragma region Spring

	AddPlayer();

#pragma endregion

#pragma region MapChip

	mapChip_ = MapChip::GetInstance();
	mapChip_->Init("resources/Level/Level1.csv");

#pragma endregion

#pragma region Enemy
	wave_ = 0;
	WaveChange();

#ifdef _DEBUG

	debugSpawn_ = std::make_unique<Entity>();
	debugSpawn_->AddComponent<ModelComp>()->AddBone("Body", ModelManager::GetInstance()->GetModel("Sphere"));

#endif // _DEBUG

#pragma endregion

#pragma region Orb

	orb_ = std::make_unique<Entity>();
	orb_->AddComponent<OrbComp>();
	orb_->Init();
	orbGauge_ = std::make_unique<Entity>();
	orbGauge_->AddComponent<OrbGaugeComp>();
	orbGauge_->Init();

	OrbGaugeComp::SetOrbComp(orb_.get());
	SoulComp::SetOrbComp(orb_.get());

#pragma endregion

#pragma region SkyCylinderComp

	skyCylinder_ = std::make_unique<SkyCylinder>();
	//skyCylinder_->AddComponent<SkyCylinderComp>();
	skyCylinder_->Init("skyCylinder");

#pragma endregion

#pragma region Camera

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Reset();

#pragma endregion

#pragma region RemainEnemy

	remainEnemy_ = std::make_unique<RemainEnemy>();
	remainEnemy_->Init();
	remainWave_ = std::make_unique<RemainWave>();
	remainWave_->Init();

#pragma endregion

	EnemyComp::StaticInit();
	isClear_ = false;
	isFinish_ = false;
}

void GameManager::Exit() {
	enemys_.clear();
	souls_.clear();
	spring_.reset();
	collisionManager_->clear();
	followCamera_.reset();
	remainEnemy_.reset();
	mapChip_->Exit();
}

void GameManager::Update(const float deltaTime) {

	slowmotionTimer_.Update(deltaTime);
	// 今の敵の数を計算
	int nowEnemyCount = 0;
	for (auto &enemy : enemys_) {
		if (enemy->GetActive()) {
			nowEnemyCount++;
		}
	}

	remainEnemy_->SetNumber(perWave_MaxEnemy_ - nowEnemyCount);
	remainEnemy_->SetMaxNumber(perWave_MaxEnemy_);
	remainEnemy_->Update();

	remainWave_->SetNumber(wave_);
	remainWave_->SetMaxNumber(maxWave_);
	remainWave_->Update();

#ifdef _DEBUG
	ImGui::Text("%d / %d", nowEnemyCount, perWave_MaxEnemy_);
#endif // _DEBUG

	if (skyCylinder_) {
		skyCylinder_->Update(deltaTime);
	}

#ifdef _DEBUG
	ImGui::Begin("Enemy");
	if (SoLib::ImGuiWidget("SpawnPos", &debugSpawn_->transform_.translate)) {
		debugSpawn_->Update(deltaTime);
	}
	if (ImGui::Button("Spawn")) {
		AddEnemy(debugSpawn_->transform_.translate);
	}
	if (ImGui::Button("KillAll")) {
		for (auto &enemy : enemys_) {
			enemy->SetActive(false);
		}
	}

	ImGui::End();

#endif // _DEBUG

	enemys_.remove_if([this](std::unique_ptr<Entity> &enemy) {
		if (!enemy->GetActive()) {
			AddSoul(enemy->GetWorldPos(), enemy->GetComponent<EnemyComp>()->GetIsStan());
			enemy->Destroy();
			enemy.reset();
			return true;
		}
		return false;
		}
	);
	souls_.remove_if([](std::unique_ptr<Entity> &soul) {
		if (!soul->GetActive()) {
			soul->Destroy();
			soul.reset();
			return true;
		}
		return false;
		}
	);

	if (spring_ && spring_->transform_.translate.y < -20.f) {
		spring_->SetActive(false);
	}

	if (spring_ && !spring_->GetActive()) {
		spring_.reset();
	}

	collisionManager_->clear();
	if (spring_) {
		collisionManager_->push_back(spring_.get());
	}
	for (auto &enemy : enemys_) {
		collisionManager_->push_back(enemy.get());
	}
	collisionManager_->ChackAllCollision();

	EnemyComp::StaticUpdate(deltaTime);

	if (!spring_) {
		AddPlayer();
	}

	if (spring_) {
		if (slowmotionTimer_.IsActive()) {
			const float slowScale = 1.f - SoLib::easeOutExpo(slowmotionTimer_.GetProgress());
			spring_->Update(deltaTime * slowScale);
		}
		else {
			spring_->Update(deltaTime);
		}
		ImGui::Text("ToGround : %f", mapChip_->GetDistanceToGround(MapChip::GlobalToLocal(spring_->transform_.translate)));
	}

	if (followCamera_) {
		followCamera_->Update(deltaTime);
	}

	for (auto &enemy : enemys_) {
		enemy->Update(deltaTime);
	}
	for (auto &soul : souls_) {
		soul->Update(deltaTime);
	}

	if (orb_) {
		orb_->transform_.translate.y = SoLib::Lerp(orb_->transform_.translate.y, followCamera_->GetCamera()->translation_.y, 0.2f);
		orb_->Update(deltaTime);
	}
	if (orbGauge_) {
		orbGauge_->Update(deltaTime);
	}

	if (perWave_MaxEnemy_ - nowEnemyCount == perWave_MaxEnemy_) {
		if (!slowmotionTimer_.IsActive()) {
			slowmotionTimer_.Start(slowCount);
		}
		if (slowmotionTimer_.IsActive() && slowmotionTimer_.IsFinish()) {
			WaveChange();
		}
	}
	ImGui::Text("wave : %d", wave_);

	// クリア条件
	if (orb_->GetComponent<OrbComp>()->GetProgress() >= 1.0f) {
		isClear_ = true;
	}

}

void GameManager::Draw() const {
	const auto &camera = *followCamera_->GetCamera();
	if (skyCylinder_) {
		skyCylinder_->Draw(camera);
	}

	if (spring_) {
		spring_->Draw(camera);
	}

	for (auto &enemy : enemys_) {
		enemy->Draw(camera);
	}
	for (auto &soul : souls_) {
		soul->Draw(camera);
	}
	if (orbGauge_) {
		orbGauge_->Draw(camera);
	}
	if (orb_) {
		orb_->Draw(camera);
	}
	mapChip_->Draw(camera);

#ifdef _DEBUG
	debugSpawn_->Draw(camera);
#endif // _DEBUG

}

void GameManager::Draw2D() const {
	remainEnemy_->Draw();
	remainWave_->Draw();

	for (auto &enemy : enemys_) {
		enemy->Draw2D();
	}
}

void GameManager::ImGuiWidget() {
	if (spring_) {
		spring_->ImGuiWidget();
	}
}

void GameManager::AddEnemy(const Vector3 &pos) {
	enemys_.push_back(std::make_unique<Entity>());
	auto &newEnemy = enemys_.back();

	++perWave_MaxEnemy_;
	newEnemy->Init();

	newEnemy->transform_.translate = pos;
	newEnemy->transform_.UpdateMatrix();
	newEnemy->AddComponent<EnemyComp>();
}

void GameManager::AddSoul(const Vector3 &pos, bool isStun) {
	souls_.push_back(std::make_unique<Entity>());
	auto &newSoul = souls_.back();

	newSoul->Init();
	newSoul->transform_.translate = pos;
	newSoul->transform_.UpdateMatrix();
	newSoul->AddComponent<SoulComp>();
	newSoul->GetComponent<SoulComp>()->SetIsStun(isStun);
	newSoul->GetComponent<SoulComp>()->ModelInit();

}

void GameManager::AddPlayer() {
	spring_ = std::make_unique<Entity>();
	spring_->Init();
	spring_->AddComponent<SpringObjectComp>();
	spring_->timeScale_ = 3.f;
	spring_->GetComponent<Rigidbody>()->hasCollider_ = true;
	spring_->transform_.translate = Vector3{ 0.f,10.f,0.f };
}

void GameManager::WaveChange() {
	if (maxWave_ <= wave_) {
		//wave_ = maxWave_;
		isFinish_ = true;
	}
	wave_++;

	WaveEnemySet(wave_);
}

void GameManager::WaveEnemySet(int wave) {
	if (wave == 1) {
		perWave_MaxEnemy_ = 0u;

		for (uint32_t i = 0u; i < 10u; ++i) {
			Vector3 pos{};
			Vector2 buff = Random::GetRandom<float>({ -18.f,20.f }, { 18.f,50.f });
			pos.x = buff.x;
			pos.y = buff.y;
			AddEnemy(pos);
		}
		EnemyComp::SetEnemyList(&enemys_);

		mapChip_->Init("resources/Level/Level1.csv");

		spring_->SetActive(false);

	}
	else if (wave == 2) {
		perWave_MaxEnemy_ = 0u;

		for (uint32_t i = 0u; i < 10u; ++i) {
			Vector3 pos{};
			Vector2 buff = Random::GetRandom<float>({ -18.f,20.f }, { 18.f,50.f });
			pos.x = buff.x;
			pos.y = buff.y;
			AddEnemy(pos);
		}
		EnemyComp::SetEnemyList(&enemys_);

		mapChip_->Init("resources/Level/Level2.csv");

		spring_->SetActive(false);


	}
	else if (wave == 3) {
		perWave_MaxEnemy_ = 0u;

		for (uint32_t i = 0u; i < 10u; ++i) {
			Vector3 pos{};
			Vector2 buff = Random::GetRandom<float>({ -18.f,20.f }, { 18.f,50.f });
			pos.x = buff.x;
			pos.y = buff.y;
			AddEnemy(pos);
		}
		EnemyComp::SetEnemyList(&enemys_);

		mapChip_->Init("resources/Level/Level3.csv");

		spring_->SetActive(false);


	}
	else if (wave == 4) {
		perWave_MaxEnemy_ = 0u;

		for (uint32_t i = 0u; i < 10u; ++i) {
			Vector3 pos{};
			Vector2 buff = Random::GetRandom<float>({ -18.f,20.f }, { 18.f,50.f });
			pos.x = buff.x;
			pos.y = buff.y;
			AddEnemy(pos);
		}
		EnemyComp::SetEnemyList(&enemys_);

		mapChip_->Init("resources/Level/Level1.csv");

		spring_->SetActive(false);

	}
	else if (wave == 5) {
		perWave_MaxEnemy_ = 0u;

		for (uint32_t i = 0u; i < 10u; ++i) {
			Vector3 pos{};
			Vector2 buff = Random::GetRandom<float>({ -18.f,20.f }, { 18.f,50.f });
			pos.x = buff.x;
			pos.y = buff.y;
			AddEnemy(pos);
		}
		EnemyComp::SetEnemyList(&enemys_);

		mapChip_->Init("resources/Level/Level2.csv");

		spring_->SetActive(false);


	}
	else if (wave == 6) {
		perWave_MaxEnemy_ = 0u;

		for (uint32_t i = 0u; i < 10u; ++i) {
			Vector3 pos{};
			Vector2 buff = Random::GetRandom<float>({ -18.f,20.f }, { 18.f,50.f });
			pos.x = buff.x;
			pos.y = buff.y;
			AddEnemy(pos);
		}
		EnemyComp::SetEnemyList(&enemys_);

		mapChip_->Init("resources/Level/Level3.csv");

		spring_->SetActive(false);


	}

}

