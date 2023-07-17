#include "ViewProjection.h"

#include "../../../Header/Create/Create.h"
#include "../../Base/DirectXCommon.h"
#include "../../../Header/Render/Render.hpp"



ViewProjection::ViewProjection()
{
}

ViewProjection::~ViewProjection()
{
}

void ViewProjection::Init()
{
	CreateConstBuffer();
	SetMap();
	UpdateMatrix();
}

void ViewProjection::CreateConstBuffer()
{
	constBuffer_ = CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(ViewProjectionMatrix));
}

void ViewProjection::SetMap()
{
	HRESULT result = constBuffer_->Map(0, nullptr, reinterpret_cast<void **>(&mapData_));
	assert(SUCCEEDED(result));
}

void ViewProjection::UpdateMatrix()
{
	matView_ = Matrix4x4::Affine(Vector3::one(), rotation_, translation_).InverseRT();
	matProjection_ = Render::MakePerspectiveFovMatrix(fovAngleY, aspectRatio, nearZ, farZ);

	mapData_->view = matView_;
	mapData_->projection = matProjection_;
	mapData_->cameraPos = translation_;
}