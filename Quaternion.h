/*********************************************************************
 * \file   Quaternion.h
 * \brief
 *
 * \author Harukichimaru
 * \date   January 2025
 * \note
 *********************************************************************/
#pragma once
#include <corecrt_math.h>

///=============================================================================
///						クオータニオン
class Quaternion {
public:
	float x;
	float y;
	float z;
	float w;
	Quaternion() : x(0), y(0), z(0), w(1) {}
	Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	// 加算演算子
	Quaternion operator+(const Quaternion &other) const {
		return { x + other.x, y + other.y, z + other.z, w + other.w };
	}
	// 減算演算子
	Quaternion operator-(const Quaternion &other) const {
		return { x - other.x, y - other.y, z - other.z, w - other.w };
	}
	// スカラー乗算演算子
	Quaternion operator*(float scalar) const {
		return { x * scalar, y * scalar, z * scalar, w * scalar };
	}
	// スカラー除算演算子
	Quaternion operator/(float scalar) const {
		return { x / scalar, y / scalar, z / scalar, w / scalar };
	}
	// 等価演算子
	bool operator==(const Quaternion &other) const {
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}
	// 非等価演算子
	bool operator!=(const Quaternion &other) const {
		return !( *this == other );
	}
	// 単項演算子
	Quaternion operator-() const {
		return { -x, -y, -z, -w };
	}
};

// 積
inline Quaternion Multiply(const Quaternion &lhs, const Quaternion &rhs) {
	return {
		lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x,
		lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w,
		lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
	};
}

//単位クォータニオンを返す
inline Quaternion Identity() {
	// 単位クォータニオンを返す
	return { 0.0f, 0.0f, 0.0f, 1.0f };
}

//共役クォータニオンを返す
inline Quaternion Conjugate(const Quaternion &q) {
	return { -q.x, -q.y, -q.z, q.w };
}

//Quernionのnormを返す
inline float Norm(const Quaternion &q) {
	return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
}

// 正規化したクォータニオンを返す
inline Quaternion Normalize(const Quaternion &q) {
	// クォータニオンの大きさを計算
	float mag = static_cast<float>(sqrtf(Norm(q)));
	// ゼロ割りを防ぐ
	if(mag != 0.0f) {
		// 正規化したクォータニオンを返す
		return { q.x / mag, q.y / mag, q.z / mag, q.w / mag };
	}
	// ゼロベクトルの場合はそのまま返す
	return q;
}

// 逆クォータニオンを返す
inline Quaternion Inverse(const Quaternion &q) {
	// 共役クォータニオンをnormで割る
	return Conjugate(q) / Norm(q);
}