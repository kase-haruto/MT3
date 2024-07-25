#include"MyFunc.h"

/// <summary>
/// ベクトルの内積を求めます
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns>ベクトルの内積</returns>
float Dot(const Vector3& v1, const Vector3& v2){
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/// <summary>
/// 正射影ベクトルを求めます
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns>計算結果を返します</returns>
Vector3 Projection(const Vector3& v1, const Vector3& v2){
	Vector3 b = Vector3::Normalize(v2);
	float d = Dot(v1, b);
	return b * d;
}

/// <summary>
/// 最近接点を求めます
/// </summary>
/// <param name="point"></param>
/// <param name="segment"></param>
/// <returns>最近接点の値を返します</returns>
Vector3 ClosestPoint(const Vector3& point, const Segment& segment){
	return segment.origin + Projection((point - segment.origin), segment.diff);
}

/// <summary>
/// 垂直ベクトルをもとめる
/// </summary>
/// <param name="v"></param>
/// <returns>垂直ベクトル</returns>
Vector3 Perpendicular(const Vector3& v){
	if (v.x != 0.0f || v.y != 0.0f){
		return {-v.y, v.x, 0.0f};
	}
	return {0.0f, -v.z, v.y};
}

/// <summary>
///	クロス積
/// </summary>
/// <returns>計算結果</returns>
Vector3 Cross(const Vector3& v1, const Vector3& v2){
	Vector3 result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

Vector3 Reflect(const Vector3& input, const Vector3& normal){
	Vector3 r;
	r = input - (normal * 2 * (Dot(input, normal)));
	return r;
}