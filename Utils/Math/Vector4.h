#pragma once

#include <cmath>
#include <immintrin.h>

struct Vector4 {

	float x;
	float y;
	float z;
	float w;

	/// <summary>
	/// ベクトルをデバック表示する
	/// </summary>
	/// <param name="x">x座標</param>
	/// <param name="y">y座標</param>
	// void Printf(int x, int y) const;

	/// <summary>
	/// ベクトル長関数
	/// </summary>
	/// <returns>ベクトルの長さ</returns>
	float Length() const {
		return std::sqrt((*this) * (*this));
	}

	/// <summary>
	/// ベクトル長関数
	/// </summary>
	/// <returns>ベクトルの長さ</returns>
	float LengthSQ() const {
		return (*this) * (*this);
	}

	/// <summary>
	/// 正規化
	/// </summary>
	/// <returns>ベクトル長が1のベクトル</returns>
	Vector4 Nomalize() const {

		float Length = this->Length();
		if (Length != 0) {
			return *this / Length;
		}
		else {
			return zero;
		}
	}

	Vector4 operator+(const Vector4 &v) const {
		__m128 res = _mm_add_ps(*(__m128 *)this, *(__m128 *) & v);
		return *(reinterpret_cast<const Vector4 *>(&res));
	}

	Vector4 operator-(const Vector4 &v) const {
		__m128 res = _mm_sub_ps(*(__m128 *)this, *(__m128 *) & v);
		return *(reinterpret_cast<const Vector4 *>(&res));
	}

	Vector4 &operator+=(const Vector4 &v) {
		__m128 res = _mm_add_ps(*(__m128 *)this, *(__m128 *) & v);
		return *this = *(reinterpret_cast<const Vector4 *>(&res));
	}
	Vector4 &operator-=(const Vector4 &v) {
		__m128 res = _mm_sub_ps(*(__m128 *)this, *(__m128 *) & v);
		return *this = *(reinterpret_cast<const Vector4 *>(&res));
	}

	Vector4 operator*(const float &value) const {
		const __m128 res =
			_mm_mul_ps(*(__m128 *)this, _mm_set_ps1(value)); // 各要素を value で乗算
		return *(reinterpret_cast<const Vector4 *>(&res));
		// float{ 0, 1, 2, 3 } の順序で構成される__m128型を、
		// float{ x, y, z, w } で構成されるVector4に解釈して変換
	}
	Vector4 operator/(const float &value) const {
		const __m128 res =
			_mm_div_ps(*(__m128 *)this, _mm_set_ps1(value)); // 各要素を value で乗算
		return *(reinterpret_cast<const Vector4 *>(&res));
		// float{ 0, 1, 2, 3 } の順序で構成される__m128型を、
		// float{ x, y, z, w } で構成されるVector4に解釈して変換
	}

	Vector4 &operator*=(const float &value) {
		return *this = *this * value;
	}

	Vector4 &operator/=(const float &value) {
		return *this = *this / value;
	}

	// Vector3 operator*(const Matrix3x3& Second) const {}

	// Vector3 operator*=(const Matrix3x3& Second) {}

	// Vector4 operator*(const Matrix4x4& Second) const;

	// Vector4& operator*=(const Matrix4x4& Second);

	// 逆ベクトル
	inline Vector4 operator-() const {
		return *this * -1;
	}

	// 内積
	inline float operator*(const Vector4 &v) const {
		return _mm_cvtss_f32(_mm_dp_ps(*(__m128 *)this, *(__m128 *) & v, 0xFF));
	}
	// 外積
	// inline float operator^(const Vector3& v) const { return x * v.y - y * v.x; }

	static const Vector4 zero;

	static const Vector4 one;

	// inline Vector4 Reflect(Vector4 normal) const {
	//	return (*this) - normal * 2 * ((*this) * normal);

	//	// return {this->x- 2}
	//}

	inline bool operator==(const Vector4 &vec) const {
		return (this->x == vec.x) && (this->y == vec.y) && (this->z == vec.z) && (this->w == vec.w);
	}

private:
};
