#include "MapChip.h"
#include <string>

#include "../../Engine/DirectBase/Model/ModelManager.h"

void MapChip::Init() {

	const float xCentor = kMapWidth_ / 2.f - 0.5f;
	//const float yCentor = kMapHight_ / 2.f - 0.5f;

	for (uint32_t x = 0u; x < kMapWidth_; ++x) {
		mapChip_[0][x].chipState_ = ChipState::box;
	}
	for (uint32_t x = 0u; x < kMapWidth_; ++x) {
	//	if (x % 7u == 0u) {
			mapChip_[5][x].chipState_ = ChipState::box;
	//	}
	}

	for (uint32_t y = 0u; y < kMapHight_; ++y) {
		for (uint32_t x = 0u; x < kMapWidth_; ++x) {

			mapChip_[y][x].transform_.translate = Vector3{ (x - xCentor) * 2.f, static_cast<float>(y) * 2.f, 0.f };
			mapChip_[y][x].transform_.UpdateMatrix();

			mapChip_[y][x].Init();

		}
	}
}

void MapChip::Draw(const Camera<Render::CameraType::Projecction> &camera) const {

	for (uint32_t y = 0u; y < kMapHight_; ++y) {
		for (uint32_t x = 0u; x < kMapWidth_; ++x) {

			mapChip_[y][x].Draw(camera);

		}
	}
}

Vector3 MapChip::HitMap(const Vector3 &beforePos, Vector3 afterPos, float radius) const {

	const Vector2 offset = Vector2{ kMapWidth_ / 2.f - 0.5f, 0.f };

	const Vector2 leftTop = (afterPos.ToVec2() + Vector2{ -radius,radius }) * 0.5f + offset;
	const Vector2 rightTop = (afterPos.ToVec2() + Vector2{ radius,radius }) * 0.5f + offset;

	const Vector2 leftDown = (afterPos.ToVec2() + Vector2{ -radius,-radius }) * 0.5f + offset;
	const Vector2 rightDown = (afterPos.ToVec2() + Vector2{ radius,-radius }) * 0.5f + offset;

	const uint32_t kChipSize = 2u;
	const float halfChip = 1.f;
	const float kExtension = 7.f / 32.f;

	bool isGround = false;


	///
	///	↓ マップチップとの当たり判定
#pragma region MapChipHit
	if (afterPos.x - beforePos.x < 0 && afterPos.y - beforePos.y < 0) { //	SA
		//				Novice::ScreenPrintf(10, 90, "SA");
		if ((mapChip_[(int)(rightDown.y)][(int)(rightDown.x)] != ChipState::air) && (mapChip_[(int)(leftTop.y)][(int)(leftTop.x)] != ChipState::air)) { //	右元と左頭が移動不可の場合
			afterPos.x = (leftTop.x + 0.01f) * kChipSize + halfChip;
			afterPos.y = (rightDown.y + 0.01f) * kChipSize + halfChip;

			// player.box.velocity.x *= 0.8;
			// player.box.velocity.y =0;
			isGround = true;
		}
		else if (mapChip_[(int)(rightDown.y)][(int)(rightDown.x)] != ChipState::air) { //	右元が移動不可の場合

			if (mapChip_[(int)(rightDown.y + 0.01f)][(int)(rightDown.x)] == ChipState::air) {	//	尚且つ右元の上マスが移動可能の場合
				afterPos.y = (rightDown.y + 0.01f) * kChipSize + halfChip;

				// player.box.velocity.y =0;
				isGround = true;
			}
		}
		else if (mapChip_[(int)(leftTop.y)][(int)(leftTop.x)] != ChipState::air) { //	左頭が移動不可の場合

			if (mapChip_[(int)(leftTop.y)][(int)(leftTop.x + 0.01f)] == ChipState::air) {  //	尚且つ左頭の右マスが開いている場合。
				afterPos.x = (leftTop.x + 0.01f) * kChipSize + halfChip;

				// player.box.velocity.x *= 0.8;
			}
		}


		else if (mapChip_[(int)(leftDown.y)][(int)(leftDown.x)] != ChipState::air) { //	左元が移動不可の場合


			//		尚且つ右と上が開いている場合
			if (mapChip_[(int)(leftDown.y)][(int)(leftDown.x + 0.01f)] == ChipState::air && mapChip_[(int)(leftDown.y + 0.01f)][(int)(leftDown.x)] == ChipState::air) {

				//if (mapChip_[(int)(leftTop.y)][(int)(leftTop.x - 0.01f)] == ChipState::air) {
				if ((((int)(afterPos.x + (halfChip - 0.01f)) % kChipSize) < ((int)(afterPos.y + (halfChip - 0.01f)) % kChipSize))) {
					afterPos.y = (leftDown.y + 0.01f) * kChipSize + halfChip;

					// player.box.velocity.y =0;
					isGround = true;
				}
				//	}
				//	else if(mapChip_[(int)(leftTop.y - 0.01f)][(int)(leftTop.x)] == ChipState::air) {
				else {
					afterPos.x = (leftDown.x + 0.01f) * kChipSize + halfChip;

					// player.box.velocity.x *= 0.8;
				}
				//	}
			}
			//		尚且つ左元の上マスが開いていて、右マスが移動不可の場合。
			else if (mapChip_[(int)(leftDown.y + 0.01f)][(int)(leftDown.x)] == ChipState::air && mapChip_[(int)(leftDown.y)][(int)(leftDown.x + 0.01f)] != ChipState::air) {
				afterPos.y = (leftDown.y + 0.01f) * kChipSize + halfChip;

				// player.box.velocity.y =0;
				isGround = true;
			}
			//		尚且つ左元の右マスが開いていて、上マスが移動不可の場合。
			else if (mapChip_[(int)(leftDown.y)][(int)(leftDown.x + 0.01f)] == ChipState::air && mapChip_[(int)(leftDown.y + 0.01f)][(int)(leftDown.x)] != ChipState::air) {
				afterPos.x = (leftDown.x + 0.01f) * kChipSize + halfChip;

				// player.box.velocity.x *= 0.8;
			}
			//		尚且つ左元の右マスと上マスが移動不可の場合。
			else if (mapChip_[(int)(leftDown.y)][(int)(leftDown.x + 0.01f)] != ChipState::air && mapChip_[(int)(leftDown.y + 0.01f)][(int)(leftDown.x)] != ChipState::air) {
				afterPos.x = (leftDown.x + 0.01f) * kChipSize + halfChip;
				afterPos.y = (leftDown.y + 0.01f) * kChipSize + halfChip;

				// player.box.velocity.x *= 0.8;
				// player.box.velocity.y =0;
				isGround = true;
			}
		}
	}


	else if (afterPos.x - beforePos.x < 0 && afterPos.y - beforePos.y > 0) { //	WA
		//				Novice::ScreenPrintf(10, 90, "WA");
		if ((mapChip_[(int)(rightTop.y)][(int)(rightTop.x)] != ChipState::air) && (mapChip_[(int)(leftDown.y)][(int)(leftDown.x)] != ChipState::air)) { //	右頭と左元が移動不可の場合
			afterPos.x = (leftDown.x + 0.01f) * kChipSize + halfChip;
			afterPos.y = (rightTop.y - 0.01f) * kChipSize + halfChip;

			// player.box.velocity.x *= 0.8;
			// player.box.velocity.y =0;
		}
		else if (mapChip_[(int)(rightTop.y)][(int)(rightTop.x)] != ChipState::air) { //	右頭が移動不可の場合

			if (mapChip_[(int)(rightTop.y)][(int)(rightTop.x - (kExtension / kChipSize))] != ChipState::air) { //	右頭の3ピクセルが移動不可の場合
				if (mapChip_[(int)(rightTop.y - 0.01f)][(int)(rightTop.x)] == ChipState::air) {	//	尚且つ右頭の下マスが移動可能の場合
					afterPos.y = (rightTop.y - 0.01f) * kChipSize + halfChip;

					// player.box.velocity.y =0;
				}
			}
			else {
				afterPos.x = (rightTop.x - 0.01f) * kChipSize + halfChip;
			}
		}
		else if (mapChip_[(int)(leftDown.y)][(int)(leftDown.x)] != ChipState::air) { //	左元が移動不可の場合

			if (mapChip_[(int)(leftDown.y)][(int)(leftDown.x + 0.01f)] == ChipState::air) {  //	尚且つ左元の右マスが開いている場合。
				afterPos.x = (leftDown.x + 0.01f) * kChipSize + halfChip;

				// player.box.velocity.x *= 0.8;
			}
		}


		else if (mapChip_[(int)(leftTop.y)][(int)(leftTop.x)] != ChipState::air) { //	左頭が移動不可の場合


			//		尚且つ右と下が開いている場合
			if (mapChip_[(int)(leftTop.y)][(int)(leftTop.x + 0.01f)] == ChipState::air && mapChip_[(int)(leftTop.y - 0.01f)][(int)(leftTop.x)] == ChipState::air) {

				//if (mapChip_[(int)(leftTop.y)][(int)(leftTop.x - 0.01f)] == ChipState::air) {
				if (((int)(afterPos.x + (halfChip - 0.01f)) % kChipSize) <= ((kChipSize)-((int)(afterPos.y + (halfChip + 0.01f)) % kChipSize)) % kChipSize) {
					afterPos.y = (leftTop.y - 0.01f) * kChipSize + halfChip;

					// player.box.velocity.y =0;
				}
				//	}
				//	else if(mapChip_[(int)(leftTop.y - 0.01f)][(int)(leftTop.x)] == ChipState::air) {
				else {
					afterPos.x = (leftTop.x + 0.01f) * kChipSize + halfChip;

					// player.box.velocity.x *= 0.8;
				}
				//	}
			}
			//		尚且つ左頭の下マスが開いていて、右マスが移動不可の場合。
			else if (mapChip_[(int)(leftTop.y - 0.01f)][(int)(leftTop.x)] == ChipState::air && mapChip_[(int)(leftTop.y)][(int)(leftTop.x + 0.01f)] != ChipState::air) {
				afterPos.y = (leftTop.y - 0.01f) * kChipSize + halfChip;

				// player.box.velocity.y =0;
			}
			//		尚且つ左頭の右マスが開いていて、下マスが移動不可の場合。
			else if (mapChip_[(int)(leftTop.y)][(int)(leftTop.x + 0.01f)] == ChipState::air && mapChip_[(int)(leftTop.y - 0.01f)][(int)(leftTop.x)] != ChipState::air) {
				afterPos.x = (leftTop.x + 0.01f) * kChipSize + halfChip;

				// player.box.velocity.x *= 0.8;
			}
			//		尚且つ左頭の右マスと下マスが移動不可の場合。
			else if (mapChip_[(int)(leftTop.y)][(int)(leftTop.x + 0.01f)] != ChipState::air && mapChip_[(int)(leftTop.y - 0.01f)][(int)(leftTop.x)] != ChipState::air) {
				afterPos.x = (leftTop.x + 0.01f) * kChipSize + halfChip;
				afterPos.y = (leftTop.y - 0.01f) * kChipSize + halfChip;

				// player.box.velocity.x *= 0.8;
				// player.box.velocity.y =0;
			}
		}
	}

	else if (afterPos.x - beforePos.x > 0 && afterPos.y - beforePos.y < 0) { //	SD
		//				Novice::ScreenPrintf(10, 90, "SD");
		if ((mapChip_[(int)(leftDown.y)][(int)(leftDown.x)] != ChipState::air) && (mapChip_[(int)(rightTop.y)][(int)(rightTop.x)] != ChipState::air)) { //	左元と右頭が移動不可の場合
			afterPos.x = (rightTop.x - 0.01f) * kChipSize + halfChip;
			afterPos.y = (leftDown.y + 0.01f) * kChipSize + halfChip;

			// player.box.velocity.x *= 0.8;
			// player.box.velocity.y =0;
			isGround = true;
		}
		else if (mapChip_[(int)(leftDown.y)][(int)(leftDown.x)] != ChipState::air) { //	左元が移動不可の場合

			if (mapChip_[(int)(leftDown.y + 0.01f)][(int)(leftDown.x)] == ChipState::air) {	//	尚且つ左元の上マスが移動可能の場合
				afterPos.y = (leftDown.y + 0.01f) * kChipSize + halfChip;

				// player.box.velocity.y =0;
				isGround = true;
			}
		}
		else if (mapChip_[(int)(rightTop.y)][(int)(rightTop.x)] != ChipState::air) { //	右頭が移動不可の場合

			if (mapChip_[(int)(rightTop.y)][(int)(rightTop.x - 0.01f)] == ChipState::air) {  //	尚且つ右頭の左マスが開いている場合。
				afterPos.x = (rightTop.x - 0.01f) * kChipSize + halfChip;

				// player.box.velocity.x *= 0.8;
			}
		}


		else if (mapChip_[(int)(rightDown.y)][(int)(rightDown.x)] != ChipState::air) { //	右元が移動不可の場合


			//		尚且つ左と上が開いている場合
			if (mapChip_[(int)(rightDown.y)][(int)(rightDown.x - 0.01f)] == ChipState::air && mapChip_[(int)(rightDown.y + 0.01f)][(int)(rightDown.x)] == ChipState::air) {

				//if (mapChip_[(int)(rightTop.y)][(int)(rightTop.x - 0.01f)] == ChipState::air) {
				if (((kChipSize)-((int)(afterPos.x + (halfChip + 0.01f)) % kChipSize)) % kChipSize < ((int)(afterPos.y + (halfChip - 0.01f)) % kChipSize)) {
					afterPos.y = (rightDown.y + 0.01f) * kChipSize + halfChip;

					// player.box.velocity.y =0;
					isGround = true;
				}
				//	}
				//	else if(mapChip_[(int)(rightTop.y - 0.01f)][(int)(rightTop.x)] == ChipState::air) {
				else {
					afterPos.x = (rightDown.x - 0.01f) * kChipSize + halfChip;
				}
				//	}
			}
			//		尚且つ右元の上マスが開いていて、左マスが移動不可の場合。
			else if (mapChip_[(int)(rightDown.y + 0.01f)][(int)(rightDown.x)] == ChipState::air && mapChip_[(int)(rightDown.y)][(int)(rightDown.x - 0.01f)] != ChipState::air) {
				afterPos.y = (rightDown.y + 0.01f) * kChipSize + halfChip;

				// player.box.velocity.y =0;
				isGround = true;
			}
			//		尚且つ右元の左マスが開いていて、上マスが移動不可の場合。
			else if (mapChip_[(int)(rightDown.y)][(int)(rightDown.x - 0.01f)] == ChipState::air && mapChip_[(int)(rightDown.y + 0.01f)][(int)(rightDown.x)] != ChipState::air) {
				afterPos.x = (rightDown.x - 0.01f) * kChipSize + halfChip;

				// player.box.velocity.x *= 0.8;
			}
			//		尚且つ右元の左マスと上マスが移動不可の場合。
			else if (mapChip_[(int)(rightDown.y)][(int)(rightDown.x - 0.01f)] != ChipState::air && mapChip_[(int)(rightDown.y + 0.01f)][(int)(rightDown.x)] != ChipState::air) {
				afterPos.x = (rightDown.x - 0.01f) * kChipSize + halfChip;
				afterPos.y = (rightDown.y + 0.01f) * kChipSize + halfChip;

				// player.box.velocity.x *= 0.8;
				// player.box.velocity.y =0;
				isGround = true;
			}
		}
	}


	else if (afterPos.x - beforePos.x > 0 && afterPos.y - beforePos.y > 0) { //	WD
		//				Novice::ScreenPrintf(10, 90, "WD");
		if ((mapChip_[(int)(leftTop.y)][(int)(leftTop.x)] != ChipState::air) && (mapChip_[(int)(rightDown.y)][(int)(rightDown.x)] != ChipState::air)) { //	左頭と右元が移動不可の場合
			afterPos.x = (rightDown.x - 0.01f) * kChipSize + halfChip;
			afterPos.y = (leftTop.y - 0.01f) * kChipSize + halfChip;

			// player.box.velocity.x *= 0.8;
			// player.box.velocity.y =0;
		}
		else if (mapChip_[(int)(leftTop.y)][(int)(leftTop.x)] != ChipState::air) { //	左頭が移動不可の場合

			if (mapChip_[(int)(leftTop.y)][(int)(leftTop.x + (kExtension / kChipSize))] != ChipState::air) { //	左頭の3ピクセルが移動不可の場合
				if (mapChip_[(int)(leftTop.y - 0.01f)][(int)(leftTop.x)] == ChipState::air) {	//	尚且つ左頭の下マスが移動可能の場合
					afterPos.y = (leftTop.y - 0.01f) * kChipSize + halfChip;

					// player.box.velocity.y =0;
				}
			}
			else {
				afterPos.x = (leftTop.x + 0.01f) * kChipSize + halfChip;
			}
		}
		else if (mapChip_[(int)(rightDown.y)][(int)(rightDown.x)] != ChipState::air) { //	右元が移動不可の場合

			if (mapChip_[(int)(rightDown.y)][(int)(rightDown.x - 0.01f)] == ChipState::air) {  //	尚且つ右元の左マスが開いている場合。
				afterPos.x = (rightDown.x - 0.01f) * kChipSize + halfChip;

				// player.box.velocity.x *= 0.8;
			}
		}


		else if (mapChip_[(int)(rightTop.y)][(int)(rightTop.x)] != ChipState::air) { //	右頭が移動不可の場合


			//		尚且つ左と下が開いている場合
			if (mapChip_[(int)(rightTop.y)][(int)(rightTop.x - 0.01f)] == ChipState::air && mapChip_[(int)(rightTop.y - 0.01f)][(int)(rightTop.x)] == ChipState::air) {

				//if (mapChip_[(int)(rightTop.y)][(int)(rightTop.x - 0.01f)] == ChipState::air) {
				if (((kChipSize)-((int)(afterPos.x + (halfChip + 0.01f)) % kChipSize)) % kChipSize <= ((kChipSize)-((int)(afterPos.y + (halfChip + 0.01f)) % kChipSize)) % kChipSize) {
					afterPos.y = (rightTop.y - 0.01f) * kChipSize + halfChip;

					// player.box.velocity.y =0;
				}
				//	}
				//	else if(mapChip_[(int)(rightTop.y - 0.01f)][(int)(rightTop.x)] == ChipState::air) {
				else {
					afterPos.x = (rightTop.x - 0.01f) * kChipSize + halfChip;

					// player.box.velocity.x *= 0.8;
				}
				//	}
			}
			//		尚且つ右頭の下マスが開いていて、左マスが移動不可の場合。
			else if (mapChip_[(int)(rightTop.y - 0.01f)][(int)(rightTop.x)] == ChipState::air && mapChip_[(int)(rightTop.y)][(int)(rightTop.x - 0.01f)] != ChipState::air) {
				afterPos.y = (rightTop.y - 0.01f) * kChipSize + halfChip;

				// player.box.velocity.y =0;
			}
			//		尚且つ右頭の左マスが開いていて、下マスが移動不可の場合。
			else if (mapChip_[(int)(rightTop.y)][(int)(rightTop.x - 0.01f)] == ChipState::air && mapChip_[(int)(rightTop.y - 0.01f)][(int)(rightTop.x)] != ChipState::air) {
				afterPos.x = (rightTop.x - 0.01f) * kChipSize + halfChip;

				// player.box.velocity.x *= 0.8;
			}
			//		尚且つ右頭の左マスと下マスが移動不可の場合。
			else if (mapChip_[(int)(rightTop.y)][(int)(rightTop.x - 0.01f)] != ChipState::air && mapChip_[(int)(rightTop.y - 0.01f)][(int)(rightTop.x)] != ChipState::air) {
				afterPos.x = (rightTop.x - 0.01f) * kChipSize + halfChip;
				afterPos.y = (rightTop.y - 0.01f) * kChipSize + halfChip;

				// player.box.velocity.x *= 0.8;
				// player.box.velocity.y =0;
			}
		}
	}

	else if (afterPos.x - beforePos.x > 0) {
		if ((mapChip_[(int)(rightTop.y)][(int)(rightTop.x)] != ChipState::air) || (mapChip_[(int)(rightDown.y)][(int)(rightDown.x)] != ChipState::air)) {
			afterPos.x = (rightTop.x - 0.01f) * kChipSize + halfChip;
		}
	}

	else if (afterPos.x - beforePos.x < 0) {
		if ((mapChip_[(int)(leftTop.y)][(int)(leftTop.x)] != ChipState::air) || (mapChip_[(int)(leftDown.y)][(int)(leftDown.x)] != ChipState::air)) {
			afterPos.x = (leftTop.x + 0.01f) * kChipSize + halfChip;
		}
	}

	else if (afterPos.y - beforePos.y > 0) {
		if ((mapChip_[(int)(leftTop.y)][(int)(leftTop.x)] != ChipState::air) && mapChip_[(int)(leftTop.y)][(int)(leftTop.x + (kExtension / kChipSize))] == ChipState::air) { //	左頭の3ピクセルが移動可の場合
			afterPos.x = (leftTop.x + 0.01f) * kChipSize + halfChip;
		}
		else if ((mapChip_[(int)(rightTop.y)][(int)(rightTop.x)] != ChipState::air) && mapChip_[(int)(rightTop.y)][(int)(rightTop.x - (kExtension / kChipSize))] == ChipState::air) { //	左頭の3ピクセルが移動可の場合
			afterPos.x = (rightTop.x - 0.01f) * kChipSize + halfChip;
		}

		else if ((mapChip_[(int)(leftTop.y)][(int)(leftTop.x)] != ChipState::air) || (mapChip_[(int)(rightTop.y)][(int)(rightTop.x)] != ChipState::air)) {
			afterPos.y = std::floor(rightTop.y - 0.01f) * kChipSize + halfChip;

			// player.box.velocity.y =0;
		}
	}

	else if (afterPos.y - beforePos.y < 0) {
		if ((mapChip_[(int)(leftDown.y)][(int)(leftDown.x)] != ChipState::air) || (mapChip_[(int)(rightDown.y)][(int)(rightDown.x)] != ChipState::air)) {
			afterPos.y = std::ceil(leftDown.y) * kChipSize;

			// player.box.velocity.y =0;

			isGround = true;

		}
	}

#pragma endregion
	///	↑ マップチップとの当たり判定
	/// 

	return afterPos;
}

void MapChip::ChipData::Init() {
	std::string modelName;

	switch (chipState_) {
	case MapChip::ChipState::air:
		modelName = "";
		break;
	case MapChip::ChipState::box:
		modelName = "box";
		break;
	default:
		break;
	}

	if (modelName == "") {
		model_ = nullptr;
	}
	else {
		model_ = ModelManager::GetInstance()->GetModel(modelName);
	}
}

void MapChip::ChipData::Draw(const Camera<Render::CameraType::Projecction> &camera) const {
	if (model_ == nullptr) { return; }

	model_->Draw(transform_, camera);
}

MapChip::ChipData &MapChip::ChipData::operator=(ChipState state) {
	chipState_ = state;
	Init();
	return *this;
}
