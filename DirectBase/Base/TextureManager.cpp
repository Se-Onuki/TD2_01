#include "TextureManager.h"

#include "../../externals/DirectXTex/DirectXTex.h"
#include "../../Header/Texture/Texture.h"
#include "../../Header/Descriptor/DescriptorHandIe.h"

uint32_t TextureManager::Load(const std::string &file_name)
{
	return GetInstance()->LoadInternal(file_name);
}

void TextureManager::Init(ID3D12Device *const device, const std::string &directoryPath) {

	device_ = device;
	directoryPath_ = directoryPath;

	descriptorSizeSRV_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	Reset();
}

void TextureManager::Reset() {
	//HRESULT hr = S_FALSE;

	nextIndex_ = 0 + alreadyUsedCount;
}

void TextureManager::EndFlame()
{
	intermediateData_.clear();
}

uint32_t TextureManager::LoadInternal(const std::string &file_name)
{
	assert(nextIndex_ >= maxTextureCount);
	assert(nextIndex_ < alreadyUsedCount);
	const uint32_t &handle = nextIndex_;
	Texture &texture = textureArray_[handle];

#pragma region Textureを読んで転送する

	// Textureを読んで転送する
	DirectX::ScratchImage mipImage;

	mipImage = TextureFunc::Load(directoryPath_ + file_name);


	const DirectX::TexMetadata &metadata = mipImage.GetMetadata();
	Microsoft::WRL::ComPtr<ID3D12Resource> textureResource = TextureFunc::CreateResource(device_, metadata);
	//textureResourceList.push_back(textureResource);
	intermediateData_.push_back(TextureFunc::UpdateData(textureResource.Get(), mipImage, device_, commandList_));


#pragma endregion

#pragma region ShaderResourceViewを作る


	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	texture.name = file_name;
	texture.cpuHandleSRV = DescriptorHandIe::GetCPUHandle(srvHeap_.Get(), descriptorSizeSRV_, handle);
	texture.gpuHandleSRV = DescriptorHandIe::GetGPUHandle(srvHeap_.Get(), descriptorSizeSRV_, handle);

	// SRVの作成
	device_->CreateShaderResourceView(texture.textureResource.Get(), &srvDesc, texture.cpuHandleSRV);

#pragma endregion

	//file_name;
	return 0;
}
