#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <array>
#include <algorithm>

#include "../Create/Create.h"
#include "DirectXCommon.h"

#include "../../../Utils/SoLib/SoLib_Traits.h"

#pragma region 単体定数バッファ

/// @brief 定数バッファ
/// @tparam T 型名
template<SoLib::IsNotPointer T>
class CBuffer final {
	// 静的警告
	static_assert(!std::is_pointer<T>::value, "CBufferに与えた型がポインタ型です");
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Resource> resources_ = nullptr;
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbView_{};

	T *mapData_;

public:

	inline ID3D12Resource *const GetResources() noexcept { return &resources_.Get(); }
	inline const ID3D12Resource *const GetResources() const noexcept { return &resources_.Get(); }

	inline const D3D12_CONSTANT_BUFFER_VIEW_DESC &GetView() const noexcept { return cbView_; }

	inline operator bool() const noexcept;		// 値が存在するか

	inline operator T &() noexcept;				// 参照
	inline operator const T &() const noexcept;	// const参照

	inline T *const operator->() noexcept;					// dataのメンバへのアクセス
	inline T *const operator->() const noexcept;		// dataのメンバへのアクセス(const)


	inline CBuffer &operator=(const T &other);	// コピー演算子

public:
	inline D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const noexcept {
		return resources_->GetGPUVirtualAddress();
	}


public:

	CBuffer();					// デフォルトコンストラクタ
	CBuffer(const CBuffer &);	// コピーコンストラクタ

	~CBuffer();

private:

	void CreateBuffer();
};

template<SoLib::IsNotPointer T>
inline CBuffer<T>::operator bool() const noexcept {
	return resources_ != nullptr;
}

template<SoLib::IsNotPointer T>
inline CBuffer<T>::operator T &() noexcept {
	return *mapData_;
}

template<SoLib::IsNotPointer T>
inline CBuffer<T>::operator const T &() const noexcept {
	return *mapData_;
}

template<SoLib::IsNotPointer T>
inline T *const CBuffer<T>::operator->() noexcept {
	return mapData_;
}

template<SoLib::IsNotPointer T>
inline T *const CBuffer<T>::operator->() const noexcept {
	return mapData_;
}

template<SoLib::IsNotPointer T>
inline CBuffer<T> &CBuffer<T>::operator=(const T &other) {
	*mapData_ = other;
	return *this;
}

template<SoLib::IsNotPointer T>
inline CBuffer<T>::CBuffer() {
	CreateBuffer();
}

template<SoLib::IsNotPointer T>
inline CBuffer<T>::CBuffer(const CBuffer &other) {
	CreateBuffer();

	// データのコピー
	*mapData_ = *other.mapData_;
}

template<SoLib::IsNotPointer T>
inline void CBuffer<T>::CreateBuffer() {
	HRESULT result = S_FALSE;


	// 256バイト単位のアライメント
	resources_ = CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), (sizeof(T) + 0xff) & ~0xff);


	cbView_.BufferLocation = resources_->GetGPUVirtualAddress();
	cbView_.SizeInBytes = static_cast<uint32_t>(resources_->GetDesc().Width);

	result = resources_->Map(0, nullptr, reinterpret_cast<void **>(&mapData_));
	assert(SUCCEEDED(result));

}

template<SoLib::IsNotPointer T>
inline CBuffer<T>::~CBuffer() {
	resources_->Release();
	//cbView_ = {};
	//mapData_ = nullptr;
}

#pragma endregion

#pragma region 配列の定数バッファ

/// @brief 定数バッファ
/// @tparam T 型名 
template<SoLib::IsNotPointer T>
class ArrayCBuffer final {
	// 静的警告
	static_assert(!std::is_pointer<T>::value, "CBufferに与えた型がポインタ型です");
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Resource> resources_ = nullptr;
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbView_{};

	size_t size_;

	T *mapData_;

public:

	inline ID3D12Resource *const GetResources() noexcept { return &resources_.Get(); }
	inline const ID3D12Resource *const GetResources() const noexcept { return &resources_.Get(); }

	inline const D3D12_CONSTANT_BUFFER_VIEW_DESC &GetView() const noexcept { return cbView_; }

	inline operator bool() const noexcept;		// 値が存在するか

	inline operator T *() noexcept;				// 参照
	inline operator const T *() const noexcept;	// const参照

	inline T &operator[](size_t index) noexcept { return mapData_[index]; }
	inline const T &operator[](size_t index) const noexcept { return mapData_[index]; }

	inline T *const operator->() noexcept;					// dataのメンバへのアクセス
	inline const T *const operator->() const noexcept;		// dataのメンバへのアクセス(const)

	size_t size() const noexcept { return size_; }
	T *const begin() const noexcept { return &mapData_[0]; }
	T *const end() const noexcept { return &mapData_[size_]; }


	template <typename U>
	inline ArrayCBuffer &operator=(const U &other);	// コピー演算子

public:
	inline D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const noexcept {
		return resources_->GetGPUVirtualAddress();
	}


public:

	ArrayCBuffer();					// デフォルトコンストラクタ

	template <typename U>
	ArrayCBuffer(const U &source);		// コピーコンストラクタ

	~ArrayCBuffer();

	/// @brief バッファの計算
	void CreateBuffer(size_t size);

	void Copy(T *const begin, T *const end) {
		std::copy(begin, end, mapData_);
	}


private:

};

template<SoLib::IsNotPointer T>
inline ArrayCBuffer<T>::operator bool() const noexcept {
	return resources_ != nullptr;
}

template<SoLib::IsNotPointer T>
inline ArrayCBuffer<T>::operator T *() noexcept {
	return *mapData_;
}

template<SoLib::IsNotPointer T>
inline ArrayCBuffer<T>::operator const T *() const noexcept {
	return *mapData_;
}

template<SoLib::IsNotPointer T>
inline T *const ArrayCBuffer<T>::operator->() noexcept {
	return *mapData_;
}

template<SoLib::IsNotPointer T>
inline const T *const ArrayCBuffer<T>::operator->() const noexcept {
	return *mapData_;
}

template<SoLib::IsNotPointer T>
template<typename U>
inline ArrayCBuffer<T> &ArrayCBuffer<T>::operator=(const U &source) {
	static_assert(requires { source.size(); }, "与えられた型にsize()メンバ関数がありません");
	static_assert(requires { source.begin(); }, "与えられた型にbegin()メンバ関数がありません");
	static_assert(requires { source.end(); }, "与えられた型にend()メンバ関数がありません");

	CreateBuffer(source.size());
	size_ = source.size();
	//Copy(source.begin(), source.end());
	std::copy(source.begin(), source.end(), mapData_);
	return *this;
}

template<SoLib::IsNotPointer T>
inline ArrayCBuffer<T>::ArrayCBuffer() :size_(0u) {
	CreateBuffer(0u);
}

template<SoLib::IsNotPointer T>
template <typename U>
inline ArrayCBuffer<T>::ArrayCBuffer(const U &source) {
	static_assert(requires { source.size(); }, "与えられた型にsize()メンバ関数がありません");
	static_assert(requires { source.begin(); }, "与えられた型にbegin()メンバ関数がありません");
	static_assert(requires { source.end(); }, "与えられた型にend()メンバ関数がありません");

	CreateBuffer(source.size());
	size_ = source.size();
	std::copy(source.begin(), source.end(), mapData_);
}

//template<SoLib::IsNotPointer T>
//inline CBuffer<T>::CBuffer(CBuffer &&other) {
//	resources_ = other.resources_;
//	cbView_ = other.cbView_;
//	*mapData_ = *other.mapData_;
//
//}


template<SoLib::IsNotPointer T>
inline void ArrayCBuffer<T>::CreateBuffer(size_t size) {
	// sizeが0以外である場合 && 現在の領域と異なる場合、領域を確保
	if (size != 0u && size_ != size) {
		HRESULT result = S_FALSE;
		if (resources_ != nullptr) { resources_->Release(); }
		// 256バイト単位のアライメント
		resources_ = CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), (sizeof(T) * size + 0xff) & ~0xff);

		cbView_.BufferLocation = resources_->GetGPUVirtualAddress();
		cbView_.SizeInBytes = static_cast<uint32_t>(resources_->GetDesc().Width);

		result = resources_->Map(0, nullptr, reinterpret_cast<void **>(&mapData_));
		assert(SUCCEEDED(result));
	}
}

template<SoLib::IsNotPointer T>
inline ArrayCBuffer<T>::~ArrayCBuffer() {
	if (resources_) {
		resources_->Release();
	}
}


#pragma endregion



#pragma region 頂点バッファ

/// @brief 頂点バッファ
/// @tparam T 頂点データの型 Index 添え字が有効か
template <SoLib::IsNotPointer T, bool Index = true>
class VertexCBuffer final {

	ArrayCBuffer<T> vertexData_;
	D3D12_VERTEX_BUFFER_VIEW vbView_;

	ArrayCBuffer<uint32_t> indexData_;
	D3D12_INDEX_BUFFER_VIEW ibView_;

public:
	VertexCBuffer() = default;
	VertexCBuffer(const VertexCBuffer &) = default;
	~VertexCBuffer() = default;

	auto &GetVertexData()  noexcept { return vertexData_; }
	const auto &GetVertexData() const noexcept { return vertexData_; }
	const auto &GetVBView() const noexcept { return vbView_; };

	auto &GetIndexData()  noexcept { return indexData_; }
	const auto &GetIndexData() const noexcept { return indexData_; }
	const auto &GetIBView() const noexcept { return ibView_; };

	template <typename U>
	void SetVertexData(const U &source);
	template <typename U>
	void SetIndexData(const U &source);
};

template <SoLib::IsNotPointer T, bool Index>
template <typename U>
void VertexCBuffer<T, Index>::SetVertexData(const U &source) {
	static_assert(requires { source.size(); }, "与えられた型にsize()メンバ関数がありません");
	static_assert(requires { source.begin(); }, "与えられた型にbegin()メンバ関数がありません");
	static_assert(requires { source.end(); }, "与えられた型にend()メンバ関数がありません");

	vertexData_ = source;

	// 頂点バッファビューを作成する
	// リソースの先頭のアドレスから使う
	vbView_.BufferLocation = vertexData_.GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点3つ分のサイズ
	vbView_.SizeInBytes = static_cast<UINT>(sizeof(T) * vertexData_.size());
	// 1頂点あたりのサイズ
	vbView_.StrideInBytes = sizeof(T);

}

template <SoLib::IsNotPointer T, bool Index>
template <typename U>
void VertexCBuffer<T, Index>::SetIndexData(const U &source) {
	static_assert(requires { source.size(); }, "与えられた型にsize()メンバ関数がありません");
	static_assert(requires { source.begin(); }, "与えられた型にbegin()メンバ関数がありません");
	static_assert(requires { source.end(); }, "与えられた型にend()メンバ関数がありません");

	indexData_ = source;

	// インデックスview
	ibView_.BufferLocation = indexData_.GetGPUVirtualAddress();
	ibView_.SizeInBytes = static_cast<UINT>(sizeof(uint32_t) * indexData_.size());
	ibView_.Format = DXGI_FORMAT_R32_UINT;
}

#pragma endregion
