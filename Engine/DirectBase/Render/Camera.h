#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <stdint.h>
#include <assert.h>

#include "Render.h"

#include "../../../Utils/Math/Matrix4x4.h"
#include "../../../Utils/Math/Vector3.h"
#include "../../../Utils/Math/Math.hpp"
#include "../Base/CBuffer.h"

#include "../Base/WinApp.h"

template <Render::CameraType T>
class Camera;

/// @brief 透視投影カメラ
template<>
class Camera<Render::CameraType::Projecction> {
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	struct CameraMatrix {
		Matrix4x4 view;
		Matrix4x4 projection;
		Vector3 cameraPos;
	};

	Camera() = default;
	~Camera() = default;

	CBuffer<CameraMatrix> constData_;

#pragma region ビュー行列の設定

	Vector3 rotation_ = { 0, 0, 0 };
	Vector3 translation_ = { 0, 0, -5 };

#pragma endregion

#pragma region 射影行列の設定
	// 垂直方向視野角
	float fovAngleY = 45.0f * Angle::Dig2Rad;
	// ビューポートのアスペクト比
	float aspectRatio = 16.f / 9;
	// 深度限界（手前側）
	float nearZ = 0.1f;
	// 深度限界（奥側）
	float farZ = 1000.0f;
#pragma endregion

	Matrix4x4 matView_{};
	Matrix4x4 matProjection_{};

	void Init();

	void CalcMatrix();

	void UpdateMatrix();

	void TransferMatrix();

	bool ImGuiWidget();

	/// @brief 画面上の座標を取得する
	/// @param pos ワールド座標
	/// @return スクリーン座標
	Vector3 GetScreenPoint(const Vector3 &pos) const;

	/// @brief ワールド座標を取得する
	/// @param pos スクリーン座標
	/// @return ワールド座標の両端[近 : 遠]
	std::pair<Vector3, Vector3> GetWorldPos(const Vector2 &pos) const;

};

/// @brief 正射影投影カメラ
template<>
class Camera<Render::CameraType::Othographic> {
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	struct CameraMatrix {
		Matrix4x4 view;
		Matrix4x4 projection;
		Vector3 cameraPos;
	};

	Camera() = default;
	~Camera() = default;

	CBuffer<CameraMatrix> constData_;

#pragma region ビュー行列の設定

	Vector3 rotation_ = { 0, 0, 0 };
	Vector3 translation_ = { 0, 0, -5 };

#pragma endregion

#pragma region 射影行列の設定

	// 画面サイズ
	Vector2 windowSize_ = { (float)WinApp::kWindowWidth,(float)WinApp::kWindowHeight };
	// 深度限界（手前側）
	float nearZ = 0.f;
	// 深度限界（奥側）
	float farZ = 100.0f;
#pragma endregion

	Matrix4x4 matView_{};
	Matrix4x4 matProjection_{};

	void Init();

	void CalcMatrix();

	void UpdateMatrix();

	void TransferMatrix();

	bool ImGuiWidget();

};