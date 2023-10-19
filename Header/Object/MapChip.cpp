#include "MapChip.h"
#include <string>

#include "../../Engine/DirectBase/Model/ModelManager.h"

void MapChip::Init() {

	//const float xCentor = kMapWidth_ / 2.f - 0.5f;
	//const float yCentor = kMapHight_ / 2.f - 0.5f;


	for (uint32_t x = 0u; x < kMapWidth_; ++x) {
		mapChip_[0][x].chipState_ = ChipState::kBox;
		mapChip_[kMapHight_ - 1][x].chipState_ = ChipState::kBox;

		if (x > kMapWidth_ / 2 - 5 && x < kMapWidth_ / 2 + 4) {
			mapChip_[0][x].chipState_ = ChipState::kUnbreakable;
		}
	}

	for (uint32_t y = 0u; y < kMapHight_; ++y) {
		mapChip_[y][0].chipState_ = ChipState::kUnbreakable;
		mapChip_[y][kMapWidth_ - 1u].chipState_ = ChipState::kUnbreakable;
	}

#pragma region マップ確保

	for (uint32_t y = 0u; y < kMapHight_; ++y) {
		for (uint32_t x = 0u; x < kMapWidth_; ++x) {

			mapChip_[y][x].Create(x, y);

		}
	}

#pragma endregion

}

void MapChip::SetCrack(uint32_t x, uint32_t y) {
	auto &chip = mapChip_[y][x];
	if (chip.chipState_ == ChipState::kBox) {
		chip.chipState_ = ChipState::kCrack;
	}
	chip.Create(x, y);
}

void MapChip::SetBreak(uint32_t x, uint32_t y) {
	auto &chip = mapChip_[y][x];
	if (chip.chipState_ == ChipState::kCrack) {
		chip.chipState_ = ChipState::kAir;
	}
	chip.Create(x, y);
}

void MapChip::Draw(const Camera<Render::CameraType::Projecction> &camera) const {

	for (uint32_t y = 0u; y < kMapHight_; ++y) {
		for (uint32_t x = 0u; x < kMapWidth_; ++x) {

			mapChip_[y][x].Draw(camera);

		}
	}
}

const MapChip::ChipData &MapChip::GetChipData(const Vector2 &vec) {

	return mapChip_[static_cast<int32_t>(vec.y)][static_cast<int32_t>(vec.x)];
}

Vector3 MapChip::HitMap(const Vector3 &beforePos, const Vector3 &afterPos, float) const {

	const Vector3 offset = Vector3{ kMapWidth_ / 2.f, 0.f,0.f } *kScale_;

	const uint32_t kChipSize = 32u;

	Vector3 resultPos = ((afterPos + offset) * kChipSize) / kScale_;

	Vector2 leftTop;
	Vector2 rightTop;

	Vector2 leftDown;
	Vector2 rightDown;

	leftTop.x = (resultPos.x - kChipSize / 2.f) / kChipSize;
	leftTop.y = (resultPos.y + kChipSize / 2.f - 1) / kChipSize;
	rightTop.x = (resultPos.x + kChipSize / 2.f - 1) / kChipSize;
	rightTop.y = (resultPos.y + kChipSize / 2.f - 1) / kChipSize;
	leftDown.x = (resultPos.x - kChipSize / 2.f) / kChipSize;
	leftDown.y = (resultPos.y - kChipSize / 2.f) / kChipSize;
	rightDown.x = (resultPos.x + kChipSize / 2.f - 1) / kChipSize;
	rightDown.y = (resultPos.y - kChipSize / 2.f) / kChipSize;

	if (
		(leftDown.x < 0.f || rightDown.x >= kMapWidth_)		// 左右が画面外
		|| (leftTop.y >= kMapHight_ || leftDown.y < 0.f)	// 上下が画面外
		) {
		return afterPos;
	}

	//leftTop /= 2.f;
	//rightTop /= 2.f;
	//leftDown /= 2.f;
	//rightDown /= 2.f;

	const float kExtension = 7.f;

	///
	///	↓ マップチップとの当たり判定
#pragma region MapChipHit
	if (afterPos.x - beforePos.x < 0 && afterPos.y - beforePos.y < 0) { //	SA
		//				Novice::ScreenPrintf(10, 90, "SA");
		if ((mapChip_[(int)(rightDown.y)][(int)(rightDown.x)] != ChipState::kAir) && (mapChip_[(int)(leftTop.y)][(int)(leftTop.x)] != ChipState::kAir)) { //	右元と左頭が移動不可の場合
			resultPos.x = (int)(leftTop.x + 1) * kChipSize + 16.f;
			resultPos.y = (int)(rightDown.y + 1) * kChipSize + 16.f;

			// player.box.velocity.x *= 0.8;
			// player.box.velocity.y = 0;
			// player.isGround = true;
		}
		else if (mapChip_[(int)(rightDown.y)][(int)(rightDown.x)] != ChipState::kAir) { //	右元が移動不可の場合

			if (mapChip_[(int)(rightDown.y) + 1][(int)(rightDown.x)] == ChipState::kAir) {	//	尚且つ右元の上マスが移動可能の場合
				resultPos.y = (int)(rightDown.y + 1) * kChipSize + 16.f;

				// player.box.velocity.y = 0;
				// player.isGround = true;
			}
		}
		else if (mapChip_[(int)(leftTop.y)][(int)(leftTop.x)] != ChipState::kAir) { //	左頭が移動不可の場合

			if (mapChip_[(int)(leftTop.y)][(int)(leftTop.x) + 1] == ChipState::kAir) {  //	尚且つ左頭の右マスが開いている場合。
				resultPos.x = (int)(leftTop.x + 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
			}
		}


		else if (mapChip_[(int)(leftDown.y)][(int)(leftDown.x)] != ChipState::kAir) { //	左元が移動不可の場合


			//		尚且つ右と上が開いている場合
			if (mapChip_[(int)(leftDown.y)][(int)(leftDown.x) + 1] == ChipState::kAir && mapChip_[(int)(leftDown.y) + 1][(int)(leftDown.x)] == ChipState::kAir) {

				//if (mapChip_[(int)(leftTop.y)][(int)(leftTop.x - 1)] == ChipState::air) {
				if ((((int)(resultPos.x + 15.f) % kChipSize) < ((int)(resultPos.y + 15.f) % kChipSize))) {
					resultPos.y = (int)(leftDown.y + 1) * kChipSize + 16.f;

					// player.box.velocity.y = 0;
					// player.isGround = true;
				}
				//	}
				//	else if(mapChip_[(int)(leftTop.y - 1)][(int)(leftTop.x)] == ChipState::air) {
				else {
					resultPos.x = (int)(leftDown.x + 1) * kChipSize + 16.f;

					// player.box.velocity.x *= 0.8;
				}
				//	}
			}
			//		尚且つ左元の上マスが開いていて、右マスが移動不可の場合。
			else if (mapChip_[(int)(leftDown.y) + 1][(int)(leftDown.x)] == ChipState::kAir && mapChip_[(int)(leftDown.y)][(int)(leftDown.x) + 1] != ChipState::kAir) {
				resultPos.y = (int)(leftDown.y + 1) * kChipSize + 16.f;

				// player.box.velocity.y = 0;
				// player.isGround = true;
			}
			//		尚且つ左元の右マスが開いていて、上マスが移動不可の場合。
			else if (mapChip_[(int)(leftDown.y)][(int)(leftDown.x) + 1] == ChipState::kAir && mapChip_[(int)(leftDown.y) + 1][(int)(leftDown.x)] != ChipState::kAir) {
				resultPos.x = (int)(leftDown.x + 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
			}
			//		尚且つ左元の右マスと上マスが移動不可の場合。
			else if (mapChip_[(int)(leftDown.y)][(int)(leftDown.x) + 1] != ChipState::kAir && mapChip_[(int)(leftDown.y) + 1][(int)(leftDown.x)] != ChipState::kAir) {
				resultPos.x = (int)(leftDown.x + 1) * kChipSize + 16.f;
				resultPos.y = (int)(leftDown.y + 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
				// player.box.velocity.y = 0;
				// player.isGround = true;
			}
		}
	}


	else if (afterPos.x - beforePos.x < 0 && afterPos.y - beforePos.y > 0) { //	WA
		//				Novice::ScreenPrintf(10, 90, "WA");
		if ((mapChip_[(int)(rightTop.y)][(int)(rightTop.x)] != ChipState::kAir) && (mapChip_[(int)(leftDown.y)][(int)(leftDown.x)] != ChipState::kAir)) { //	右頭と左元が移動不可の場合
			resultPos.x = (int)(leftDown.x + 1) * kChipSize + 16.f;
			resultPos.y = (int)(rightTop.y - 1) * kChipSize + 16.f;

			// player.box.velocity.x *= 0.8;
			// player.box.velocity.y = 0;
		}
		else if (mapChip_[(int)(rightTop.y)][(int)(rightTop.x)] != ChipState::kAir) { //	右頭が移動不可の場合

			if (mapChip_[(int)(rightTop.y)][(int)(rightTop.x - (kExtension / kChipSize))] != ChipState::kAir) { //	右頭の3ピクセルが移動不可の場合
				if (mapChip_[(int)(rightTop.y) - 1][(int)(rightTop.x)] == ChipState::kAir) {	//	尚且つ右頭の下マスが移動可能の場合
					resultPos.y = (int)(rightTop.y - 1) * kChipSize + 16.f;

					// player.box.velocity.y = 0;
				}
			}
			else {
				resultPos.x = (int)(rightTop.x - 1) * kChipSize + 16.f;
			}
		}
		else if (mapChip_[(int)(leftDown.y)][(int)(leftDown.x)] != ChipState::kAir) { //	左元が移動不可の場合

			if (mapChip_[(int)(leftDown.y)][(int)(leftDown.x) + 1] == ChipState::kAir) {  //	尚且つ左元の右マスが開いている場合。
				resultPos.x = (int)(leftDown.x + 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
			}
		}


		else if (mapChip_[(int)(leftTop.y)][(int)(leftTop.x)] != ChipState::kAir) { //	左頭が移動不可の場合


			//		尚且つ右と下が開いている場合
			if (mapChip_[(int)(leftTop.y)][(int)(leftTop.x) + 1] == ChipState::kAir && mapChip_[(int)(leftTop.y) - 1][(int)(leftTop.x)] == ChipState::kAir) {

				//if (mapChip_[(int)(leftTop.y)][(int)(leftTop.x - 1)] == ChipState::air) {
				if (((int)(resultPos.x + 15.f) % kChipSize) <= ((kChipSize)-((int)(resultPos.y + 17.f) % kChipSize)) % kChipSize) {
					resultPos.y = (int)(leftTop.y - 1) * kChipSize + 16.f;

					// player.box.velocity.y = 0;
				}
				//	}
				//	else if(mapChip_[(int)(leftTop.y - 1)][(int)(leftTop.x)] == ChipState::air) {
				else {
					resultPos.x = (int)(leftTop.x + 1) * kChipSize + 16.f;

					// player.box.velocity.x *= 0.8;
				}
				//	}
			}
			//		尚且つ左頭の下マスが開いていて、右マスが移動不可の場合。
			else if (mapChip_[(int)(leftTop.y) - 1][(int)(leftTop.x)] == ChipState::kAir && mapChip_[(int)(leftTop.y)][(int)(leftTop.x) + 1] != ChipState::kAir) {
				resultPos.y = (int)(leftTop.y - 1) * kChipSize + 16.f;

				// player.box.velocity.y = 0;
			}
			//		尚且つ左頭の右マスが開いていて、下マスが移動不可の場合。
			else if (mapChip_[(int)(leftTop.y)][(int)(leftTop.x) + 1] == ChipState::kAir && mapChip_[(int)(leftTop.y) - 1][(int)(leftTop.x)] != ChipState::kAir) {
				resultPos.x = (int)(leftTop.x + 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
			}
			//		尚且つ左頭の右マスと下マスが移動不可の場合。
			else if (mapChip_[(int)(leftTop.y)][(int)(leftTop.x) + 1] != ChipState::kAir && mapChip_[(int)(leftTop.y) - 1][(int)(leftTop.x)] != ChipState::kAir) {
				resultPos.x = (int)(leftTop.x + 1) * kChipSize + 16.f;
				resultPos.y = (int)(leftTop.y - 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
				// player.box.velocity.y = 0;
			}
		}
	}

	else if (afterPos.x - beforePos.x > 0 && afterPos.y - beforePos.y < 0) { //	SD
		//				Novice::ScreenPrintf(10, 90, "SD");
		if ((mapChip_[(int)(leftDown.y)][(int)(leftDown.x)] != ChipState::kAir) && (mapChip_[(int)(rightTop.y)][(int)(rightTop.x)] != ChipState::kAir)) { //	左元と右頭が移動不可の場合
			resultPos.x = (int)(rightTop.x - 1) * kChipSize + 16.f;
			resultPos.y = (int)(leftDown.y + 1) * kChipSize + 16.f;

			// player.box.velocity.x *= 0.8;
			// player.box.velocity.y = 0;
			// player.isGround = true;
		}
		else if (mapChip_[(int)(leftDown.y)][(int)(leftDown.x)] != ChipState::kAir) { //	左元が移動不可の場合

			if (mapChip_[(int)(leftDown.y) + 1][(int)(leftDown.x)] == ChipState::kAir) {	//	尚且つ左元の上マスが移動可能の場合
				resultPos.y = (int)(leftDown.y + 1) * kChipSize + 16.f;

				// player.box.velocity.y = 0;
				// player.isGround = true;
			}
		}
		else if (mapChip_[(int)(rightTop.y)][(int)(rightTop.x)] != ChipState::kAir) { //	右頭が移動不可の場合

			if (mapChip_[(int)(rightTop.y)][(int)(rightTop.x) - 1] == ChipState::kAir) {  //	尚且つ右頭の左マスが開いている場合。
				resultPos.x = (int)(rightTop.x - 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
			}
		}


		else if (mapChip_[(int)(rightDown.y)][(int)(rightDown.x)] != ChipState::kAir) { //	右元が移動不可の場合


			//		尚且つ左と上が開いている場合
			if (mapChip_[(int)(rightDown.y)][(int)(rightDown.x) - 1] == ChipState::kAir && mapChip_[(int)(rightDown.y) + 1][(int)(rightDown.x)] == ChipState::kAir) {

				//if (mapChip_[(int)(rightTop.y)][(int)(rightTop.x - 1)] == ChipState::air) {
				if (((kChipSize)-((int)(resultPos.x + 17.f) % kChipSize)) % kChipSize < ((int)(resultPos.y + 15.f) % kChipSize)) {
					resultPos.y = (int)(rightDown.y + 1) * kChipSize + 16.f;

					// player.box.velocity.y = 0;
					// player.isGround = true;
				}
				//	}
				//	else if(mapChip_[(int)(rightTop.y - 1)][(int)(rightTop.x)] == ChipState::air) {
				else {
					resultPos.x = (int)(rightDown.x - 1) * kChipSize + 16.f;
				}
				//	}
			}
			//		尚且つ右元の上マスが開いていて、左マスが移動不可の場合。
			else if (mapChip_[(int)(rightDown.y) + 1][(int)(rightDown.x)] == ChipState::kAir && mapChip_[(int)(rightDown.y)][(int)(rightDown.x) - 1] != ChipState::kAir) {
				resultPos.y = (int)(rightDown.y + 1) * kChipSize + 16.f;

				// player.box.velocity.y = 0;
				// player.isGround = true;
			}
			//		尚且つ右元の左マスが開いていて、上マスが移動不可の場合。
			else if (mapChip_[(int)(rightDown.y)][(int)(rightDown.x) - 1] == ChipState::kAir && mapChip_[(int)(rightDown.y) + 1][(int)(rightDown.x)] != ChipState::kAir) {
				resultPos.x = (int)(rightDown.x - 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
			}
			//		尚且つ右元の左マスと上マスが移動不可の場合。
			else if (mapChip_[(int)(rightDown.y)][(int)(rightDown.x) - 1] != ChipState::kAir && mapChip_[(int)(rightDown.y) + 1][(int)(rightDown.x)] != ChipState::kAir) {
				resultPos.x = (int)(rightDown.x - 1) * kChipSize + 16.f;
				resultPos.y = (int)(rightDown.y + 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
				// player.box.velocity.y = 0;
				// player.isGround = true;
			}
		}
	}


	else if (afterPos.x - beforePos.x > 0 && afterPos.y - beforePos.y > 0) { //	WD
		//				Novice::ScreenPrintf(10, 90, "WD");
		if ((mapChip_[(int)(leftTop.y)][(int)(leftTop.x)] != ChipState::kAir) && (mapChip_[(int)(rightDown.y)][(int)(rightDown.x)] != ChipState::kAir)) { //	左頭と右元が移動不可の場合
			resultPos.x = (int)(rightDown.x - 1) * kChipSize + 16.f;
			resultPos.y = (int)(leftTop.y - 1) * kChipSize + 16.f;

			// player.box.velocity.x *= 0.8;
			// player.box.velocity.y = 0;
		}
		else if (mapChip_[(int)(leftTop.y)][(int)(leftTop.x)] != ChipState::kAir) { //	左頭が移動不可の場合

			if (mapChip_[(int)(leftTop.y)][(int)(leftTop.x + (kExtension / kChipSize))] != ChipState::kAir) { //	左頭の3ピクセルが移動不可の場合
				if (mapChip_[(int)(leftTop.y) - 1][(int)(leftTop.x)] == ChipState::kAir) {	//	尚且つ左頭の下マスが移動可能の場合
					resultPos.y = (int)(leftTop.y - 1) * kChipSize + 16.f;

					// player.box.velocity.y = 0;
				}
			}
			else {
				resultPos.x = (int)(leftTop.x + 1) * kChipSize + 16.f;
			}
		}
		else if (mapChip_[(int)(rightDown.y)][(int)(rightDown.x)] != ChipState::kAir) { //	右元が移動不可の場合

			if (mapChip_[(int)(rightDown.y)][(int)(rightDown.x) - 1] == ChipState::kAir) {  //	尚且つ右元の左マスが開いている場合。
				resultPos.x = (int)(rightDown.x - 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
			}
		}


		else if (mapChip_[(int)(rightTop.y)][(int)(rightTop.x)] != ChipState::kAir) { //	右頭が移動不可の場合


			//		尚且つ左と下が開いている場合
			if (mapChip_[(int)(rightTop.y)][(int)(rightTop.x) - 1] == ChipState::kAir && mapChip_[(int)(rightTop.y) - 1][(int)(rightTop.x)] == ChipState::kAir) {

				//if (mapChip_[(int)(rightTop.y)][(int)(rightTop.x - 1)] == ChipState::air) {
				if (((kChipSize)-((int)(resultPos.x + 17.f) % kChipSize)) % kChipSize <= ((kChipSize)-((int)(resultPos.y + 17.f) % kChipSize)) % kChipSize) {
					resultPos.y = (int)(rightTop.y - 1) * kChipSize + 16.f;

					// player.box.velocity.y = 0;
				}
				//	}
				//	else if(mapChip_[(int)(rightTop.y - 1)][(int)(rightTop.x)] == ChipState::air) {
				else {
					resultPos.x = (int)(rightTop.x - 1) * kChipSize + 16.f;

					// player.box.velocity.x *= 0.8;
				}
				//	}
			}
			//		尚且つ右頭の下マスが開いていて、左マスが移動不可の場合。
			else if (mapChip_[(int)(rightTop.y) - 1][(int)(rightTop.x)] == ChipState::kAir && mapChip_[(int)(rightTop.y)][(int)(rightTop.x) - 1] != ChipState::kAir) {
				resultPos.y = (int)(rightTop.y - 1) * kChipSize + 16.f;

				// player.box.velocity.y = 0;
			}
			//		尚且つ右頭の左マスが開いていて、下マスが移動不可の場合。
			else if (mapChip_[(int)(rightTop.y)][(int)(rightTop.x) - 1] == ChipState::kAir && mapChip_[(int)(rightTop.y) - 1][(int)(rightTop.x)] != ChipState::kAir) {
				resultPos.x = (int)(rightTop.x - 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
			}
			//		尚且つ右頭の左マスと下マスが移動不可の場合。
			else if (mapChip_[(int)(rightTop.y)][(int)(rightTop.x) - 1] != ChipState::kAir && mapChip_[(int)(rightTop.y) - 1][(int)(rightTop.x)] != ChipState::kAir) {
				resultPos.x = (int)(rightTop.x - 1) * kChipSize + 16.f;
				resultPos.y = (int)(rightTop.y - 1) * kChipSize + 16.f;

				// player.box.velocity.x *= 0.8;
				// player.box.velocity.y = 0;
			}
		}
	}

	else if (afterPos.x - beforePos.x > 0) {
		if ((mapChip_[(int)(rightTop.y)][(int)(rightTop.x)] != ChipState::kAir) || (mapChip_[(int)(rightDown.y)][(int)(rightDown.x)] != ChipState::kAir)) {
			resultPos.x = (int)(rightTop.x - 1) * kChipSize + 16.f;
		}
	}

	else if (afterPos.x - beforePos.x < 0) {
		if ((mapChip_[(int)(leftTop.y)][(int)(leftTop.x)] != ChipState::kAir) || (mapChip_[(int)(leftDown.y)][(int)(leftDown.x)] != ChipState::kAir)) {
			resultPos.x = (int)(leftTop.x + 1) * kChipSize + 16.f;
		}
	}

	else if (afterPos.y - beforePos.y > 0) {
		if ((mapChip_[(int)(leftTop.y)][(int)(leftTop.x)] != ChipState::kAir) && mapChip_[(int)(leftTop.y)][(int)(leftTop.x + (kExtension / kChipSize))] == ChipState::kAir) { //	左頭の3ピクセルが移動可の場合
			resultPos.x = (int)(leftTop.x + 1) * kChipSize + 16.f;
		}
		else if ((mapChip_[(int)(rightTop.y)][(int)(rightTop.x)] != ChipState::kAir) && mapChip_[(int)(rightTop.y)][(int)(rightTop.x - (kExtension / kChipSize))] == ChipState::kAir) { //	左頭の3ピクセルが移動可の場合
			resultPos.x = (int)(rightTop.x - 1) * kChipSize + 16.f;
		}

		else if ((mapChip_[(int)(leftTop.y)][(int)(leftTop.x)] != ChipState::kAir) || (mapChip_[(int)(rightTop.y)][(int)(rightTop.x)] != ChipState::kAir)) {
			resultPos.y = (int)(rightTop.y - 1) * kChipSize + 16.f;

			// player.box.velocity.y = 0;
		}
	}

	else if (afterPos.y - beforePos.y < 0) {
		if ((mapChip_[(int)(leftDown.y)][(int)(leftDown.x)] != ChipState::kAir) || (mapChip_[(int)(rightDown.y)][(int)(rightDown.x)] != ChipState::kAir)) {
			resultPos.y = (int)(leftDown.y + 1) * kChipSize + 16.f;

			// player.box.velocity.y = 0;

			// player.isGround = true;

		}
	}
	else {
		return afterPos;
	}

#pragma endregion
	///	↑ マップチップとの当たり判定
	/// 

	return (resultPos * kScale_) / kChipSize - offset;

}

void MapChip::ChipData::Init() {
	std::string modelName;

	switch (chipState_) {
	case MapChip::ChipState::kAir:
		modelName = "";
		break;
	case MapChip::ChipState::kBox:
		modelName = "Box";
		break;
	case MapChip::ChipState::kUnbreakable:
		modelName = "Unbreakable";
		break;
	case MapChip::ChipState::kCrack:
		modelName = "Crack";
		break;
	default:
		modelName = "";
		break;
	}

	if (modelName == "") {
		model_ = nullptr;
	}
	else {
		if (!transform_) {
			transform_ = std::make_unique<Transform>();
		}
		model_ = ModelManager::GetInstance()->GetModel(modelName);
	}
}

void MapChip::ChipData::Create(uint32_t x, uint32_t y) {

	Init();

	const float xCentor = kMapWidth_ / 2.f - 0.5f;
	if (transform_) {
		transform_->translate = Vector3{ (x - xCentor) * 2.f, static_cast<float>(y) * 2.f + 1.f, 0.f };
		transform_->UpdateMatrix();
	}
}

void MapChip::ChipData::Draw(const Camera<Render::CameraType::Projecction> &camera) const {
	if (model_ == nullptr) { return; }
	if (transform_) {
		model_->Draw(*transform_.get(), camera);
	}
}

MapChip::ChipData &MapChip::ChipData::operator=(ChipState state) {
	chipState_ = state;
	Init();
	return *this;
}
