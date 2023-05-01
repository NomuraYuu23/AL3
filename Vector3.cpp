#include "Vector3.h"
#include <cmath>

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2) {

	Vector3 result;

	result = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};

	return result;
}

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {

	Vector3 result;

	result = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};

	return result;
}

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v) {

	Vector3 result;

	result = {scalar * v.x, scalar * v.y, scalar * v.z};

	return result;
}

// 内積
float Dot(const Vector3& v1, const Vector3& v2) {

	float result;

	result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	return result;
}

// 長さ（ノルム）
float Length(const Vector3& v) {

	float result;

	result = std::sqrtf(Dot(v, v));

	return result;
}

// 正規化
Vector3 Normalize(const Vector3& v) {

	Vector3 result;
	float norm;

	norm = Length(v);

	if (v.x != 0.0) {
		result.x = v.x / norm;
	} else {
		result.x = 0.0f;
	}

	if (v.y != 0.0) {
		result.y = v.y / norm;
	} else {
		result.y = 0.0f;
	}

	if (v.z != 0.0) {
		result.z = v.z / norm;
	} else {
		result.z = 0.0f;
	}

	return result;
}

// 線形補間
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {

	Vector3 result = Add(v1, Multiply(t, Subtract(v2, v1)));

	return result;

}

// 球面線形補間
Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t) {

	//v1,v2を正規化
	Vector3 start = Normalize(v1);
	Vector3 end = Normalize(v2);

	//2ベクトル間の角度(鋭角側)
	float angle = std::acosf(Dot(start, end));

	//補間係数
	float Ps = std::sinf(angle * (1 - t));
	float Pe = std::sinf(angle * t);

	Vector3 result = Multiply( 1.0f / std::sinf(angle), Add( Multiply(Ps, start), Multiply(Pe, end)));

	return result;

}
