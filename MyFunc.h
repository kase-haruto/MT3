#pragma once
#include"MyStruct.h"

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