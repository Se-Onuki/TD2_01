#include "SpringObjectComp.h"
#include "ModelComp.h"
#include "../../../Engine/DirectBase/Model/ModelManager.h"
#include "Rigidbody.h"

#include "../../../Utils/SoLib/SoLib_Lerp.h"
#include "Collider.h"
#include "EnemyComp.h"
#include "../../../Engine/DirectBase/File/GlobalVariables.h"

#include "../../../Engine/DirectBase/Base/Audio.h"

uint32_t SpringObjectComp::jumpSE = 0u;
uint32_t SpringObjectComp::bindSE = 0u;
SpringObjectComp::~SpringObjectComp() {
}

void SpringObjectComp::Init() {
	input_ = Input::GetInstance();
	ApplyVariables(groupName_.c_str());

	auto *const springModel = ModelManager::GetInstance()->GetModel("Spring");
	auto *const modelComp = object_->AddComponent<ModelComp>();
	modelComp->AddBone("Body", springModel, Transform{ .translate{0.f,-1.f,0.f} });

	auto *const rigidbody = object_->AddComponent<Rigidbody>();
	rigidbody->SetMaxSpeed({ 3.f,1.f,0.f });

	auto *const colliderComp = object_->AddComponent<ColliderComp>();
	colliderComp->SetRadius(vHitBox);
	colliderComp->SetCentor({ 0.f,vHitBox.GetItem() * 0.5f,0.f });
	colliderComp->SetCollisionAttribute(static_cast<uint32_t>(CollisionFilter::Player));
	colliderComp->SetCollisionMask(~static_cast<uint32_t>(CollisionFilter::Player));

	if (jumpSE == 0u) {
		jumpSE = Audio::GetInstance()->LoadWave("resources/Sounds/jump.wav");
		bindSE = Audio::GetInstance()->LoadWave("resources/Sounds/bind.wav");
	}
	//blockBreakSE = Audio::GetInstance()->LoadWave("resources/Sounds/brockBreak.wav");

	state_ = std::make_unique<PlayerStateManager>(this);
	state_->Init();

	AddVariable(groupName_.c_str());
}

void SpringObjectComp::Update([[maybe_unused]] float deltaTime) {
	ApplyVariables(groupName_.c_str());
	state_->Update(deltaTime);

	auto *const colliderComp = object_->GetComponent<ColliderComp>();
	colliderComp->SetRadius(vHitBox);
	colliderComp->SetCentor({ 0.f,vHitBox.GetItem() * 0.5f,0.f });

	auto *const rigidbody = object_->GetComponent<Rigidbody>();

	if (!dynamic_cast<const SpawnState *>(state_->GetState())) {

		// 毎フレームかかる処理はdeltaTimeをかける
		rigidbody->ApplyContinuousForce(Vector3::up * -9.8f, deltaTime);

	}
	Vector3 vec = rigidbody->GetVelocity();
	vec.x = std::clamp(vec.x, -static_cast<float>(vMaxSpeed_->x), static_cast<float>(vMaxSpeed_->x));
	rigidbody->SetVelocity(vec);

}

void SpringObjectComp::OnCollision(Entity *const other) {
	state_->OnCollision(other);
}

void SpringObjectComp::ApplyVariables(const char *const groupName) {
	const GlobalVariables *const gVariable = GlobalVariables::GetInstance();
	const auto &cGroup = gVariable->GetGroup(groupName);

	cGroup >> vJumpString_;
	cGroup >> vMoveString_;
	cGroup >> vInvincibleTime_;
	cGroup >> vMaxSpeed_;
	cGroup >> vSquatScale_;
	cGroup >> vSquatTime_;
	cGroup >> vJumpAnimTime_;
	cGroup >> vLandingTime_;
	cGroup >> vHitBox;
	cGroup >> vInputDisableHeight;
	cGroup >> vSpawnTime_;
}

void SpringObjectComp::AddVariable(const char *const groupName) const {
	GlobalVariables *const gVariable = GlobalVariables::GetInstance();
	auto &group = gVariable->GetGroup(groupName);

	group << vJumpString_;
	group << vMoveString_;
	group << vInvincibleTime_;
	group << vMaxSpeed_;
	group << vSquatScale_;
	group << vSquatTime_;
	group << vJumpAnimTime_;
	group << vLandingTime_;
	group << vHitBox;
	group << vInputDisableHeight;
	group << vSpawnTime_;
}

void DefaultState::Init([[maybe_unused]] float deltaTime) {
	stateTimer_.Start(stateManager_->parent_->vLandingTime_);
	startModelScale_ = stateManager_->parent_->vSquatScale_;
}

void DefaultState::Update([[maybe_unused]] float deltaTime) {
	if (Input::GetInstance()->GetDirectInput()->IsPress(DIK_SPACE)) {
		stateManager_->ChangeState<SquattingState>();
	}

#pragma region モデルアニメーション

	// タイマーの更新
	stateTimer_.Update(deltaTime);

	const Vector3 squatScale = Vector3::one;

	Vector3 &modelScale = stateManager_->parent_->object_->GetComponent<ModelComp>()->GetBone("Body")->transform_.scale;

	modelScale = SoLib::Lerp(startModelScale_, squatScale, SoLib::easeOutElastic(stateTimer_.GetProgress()));

#pragma endregion

}

void FallingState::Init([[maybe_unused]] float deltaTime) {
	auto *const rigidbody = stateManager_->parent_->object_->GetComponent<Rigidbody>();
	rigidbody->SetVelocity(Vector3::zero);
	rigidbody->SetAcceleration(Vector3::up * -10.f);
}

void FallingState::Update([[maybe_unused]] float deltaTime) {
	if (stateManager_->parent_->object_->GetComponent<Rigidbody>()->GetIsGround()) {
		stateManager_->ChangeState<DefaultState>();

		Vector3 selfPos = stateManager_->parent_->object_->GetWorldPos();
		selfPos.y -= 1.f;
		Vector2 downPos = MapChip::GlobalToLocal(selfPos);
		// マップチップ
		auto *const mapChip = MapChip::GetInstance();
		// もし足元がブロックならばヒビを入れる
		mapChip->SetCrack(static_cast<uint32_t>(downPos.x), static_cast<uint32_t>(downPos.y));
		//Audio::GetInstance()->PlayWave(stateManager_->parent_->GetBlockBreakSE(), false, 0.3f);

		// もし空気ならば左右にヒビを入れる
		if (mapChip->GetChipData(downPos) == MapChip::ChipState::kAir) {
			mapChip->SetCrack(static_cast<uint32_t>(downPos.x + 0.5f), static_cast<uint32_t>(downPos.y));
			mapChip->SetCrack(static_cast<uint32_t>(downPos.x - 0.5f), static_cast<uint32_t>(downPos.y));
		}
	}

}

void FallingState::Exit([[maybe_unused]] float deltaTime) {
}

void FallingState::OnCollision([[maybe_unused]] Entity *const other) {
	auto *const enemyComp = other->GetComponent<EnemyComp>();
	if (enemyComp) {
		// スタン中の敵全破壊
		enemyComp->BreakAll();
	}
}

void JumpingState::Init([[maybe_unused]] float deltaTime) {
	auto *const rigidbody = stateManager_->parent_->object_->GetComponent<Rigidbody>();
	rigidbody->SetVelocity(Vector3::zero);
	rigidbody->ApplyInstantForce(Vector3::up * stateManager_->parent_->vJumpString_);
	rigidbody->SetIsGround(false);


	Vector3 selfPos = stateManager_->parent_->object_->GetWorldPos();
	selfPos.y -= 1.f;
	Vector2 downPos = MapChip::GlobalToLocal(selfPos);
	// マップチップ
	auto *const mapChip = MapChip::GetInstance();

	mapChip->SetBreak(static_cast<uint32_t>(downPos.x), static_cast<uint32_t>(downPos.y));
	// もし空気ならば左右にヒビを入れる
	if (mapChip->GetChipData(downPos) == MapChip::ChipState::kAir) {
		mapChip->SetBreak(static_cast<uint32_t>(downPos.x + 0.5f), static_cast<uint32_t>(downPos.y));
		mapChip->SetBreak(static_cast<uint32_t>(downPos.x - 0.5f), static_cast<uint32_t>(downPos.y));
	}

	stateTimer_.Start(stateManager_->parent_->vJumpAnimTime_);
	startModelScale_ = stateManager_->parent_->vSquatScale_;
}

void JumpingState::Update([[maybe_unused]] float deltaTime) {

	auto *const rigidbody = stateManager_->parent_->object_->GetComponent<Rigidbody>();
	if (Input::GetInstance()->GetDirectInput()->IsPress(DIK_A)) {
		rigidbody->ApplyContinuousForce(Vector3::right * stateManager_->parent_->vMoveString_ * -1.f, deltaTime);
	}
	if (Input::GetInstance()->GetDirectInput()->IsPress(DIK_D)) {
		rigidbody->ApplyContinuousForce(Vector3::right * stateManager_->parent_->vMoveString_, deltaTime);
	}

	if (stateManager_->parent_->object_->GetComponent<Rigidbody>()->GetIsGround()) {
		stateManager_->ChangeState<DefaultState>();
	}

	if (Input::GetInstance()->GetDirectInput()->IsPress(DIK_SPACE) && MapChip::GetInstance()->GetDistanceToGround(MapChip::GlobalToLocal(stateManager_->parent_->object_->GetWorldPos())) > stateManager_->parent_->vInputDisableHeight) {
		stateManager_->ChangeState<FallingState>();
	}

#pragma region モデルアニメーション

	// タイマーの更新
	stateTimer_.Update(deltaTime);

	const Vector3 jumpAnimScale = Vector3::one;

	Vector3 &modelScale = stateManager_->parent_->object_->GetComponent<ModelComp>()->GetBone("Body")->transform_.scale;

	modelScale = SoLib::Lerp(startModelScale_, jumpAnimScale, easeFunc(stateTimer_.GetProgress()));

#pragma endregion



}

void JumpingState::OnCollision([[maybe_unused]] Entity *const other) {

	auto *const rigidbody = stateManager_->parent_->object_->GetComponent<Rigidbody>();


	// 敵のコンポーネントを取得
	auto *const enemyComp = other->GetComponent<EnemyComp>();
	if (enemyComp) {
		// 上から踏まれた場合
		if (rigidbody->GetVelocity().y <= 0.f) {
			// 跳ね返りSEの再生
			Audio::GetInstance()->PlayWave(stateManager_->parent_->GetBindSE(), false, 0.3f);

			stateManager_->ChangeState<JumpingState>();

			enemyComp->StartStan();

		}
		// 下から叩かれた場合
		else if (stateManager_->parent_->object_->transform_.translate.y <= other->transform_.translate.y) {

			// 自分自身がスタンしてる場合
			if (enemyComp->GetIsStan()) {
				// スタン中の敵全破壊
				enemyComp->BreakAll();
			}
			else {
				other->SetActive(false);
			}

		}
	}

}

void SquattingState::Init([[maybe_unused]] float deltaTime) {
	stateTimer_.Start(stateManager_->parent_->vSquatTime_);
	startModelScale_ = stateManager_->parent_->object_->GetComponent<ModelComp>()->GetBone("Body")->transform_.scale;
}

void SquattingState::Update([[maybe_unused]] float deltaTime) {
	// タイマーの更新
	stateTimer_.Update(deltaTime);

	if (Input::GetInstance()->GetDirectInput()->IsRelease(DIK_SPACE)) {
		Audio::GetInstance()->PlayWave(stateManager_->parent_->GetJumpSE(), false, 0.3f);
		stateManager_->ChangeState<JumpingState>();
	}
	const Vector3 &squatScale = stateManager_->parent_->vSquatScale_;

	Vector3 &modelScale = stateManager_->parent_->object_->GetComponent<ModelComp>()->GetBone("Body")->transform_.scale;

	modelScale = SoLib::Lerp(startModelScale_, squatScale, SoLib::easeOutElastic(stateTimer_.GetProgress()));

}

void SpawnState::Init([[maybe_unused]] float deltaTime) {
	stateTimer_.Start(stateManager_->parent_->vSpawnTime_);
	stateManager_->parent_->object_->GetComponent<ModelComp>()->GetBone("Body")->transform_.scale = Vector3::zero;
	startModelScale_ = stateManager_->parent_->object_->GetComponent<ModelComp>()->GetBone("Body")->transform_.scale;
}

void SpawnState::Update(float deltaTime) {
	// タイマーの更新
	stateTimer_.Update(deltaTime);

	Vector3 &modelScale = stateManager_->parent_->object_->GetComponent<ModelComp>()->GetBone("Body")->transform_.scale;

	modelScale = SoLib::Lerp(startModelScale_, Vector3::one, SoLib::easeOutBounce(stateTimer_.GetProgress()));

	if (stateTimer_.IsActive() && stateTimer_.IsFinish()) {
		stateManager_->ChangeState<FallingState>();
	}

}
