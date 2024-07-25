#pragma once
#include"MyStruct.h"
#include<cmath>

/// <summary>
/// ベクトルの内積を求めます
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns>ベクトルの内積</returns>
float Dot(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 正射影ベクトルを求めます
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns>計算結果を返します</returns>
Vector3 Projection(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 最近接点を求めます
/// </summary>
/// <param name="point"></param>
/// <param name="segment"></param>
/// <returns>最近接点の値を返します</returns>
Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

/// <summary>
/// 垂直ベクトルをもとめる
/// </summary>
/// <param name="v"></param>
/// <returns>垂直ベクトル</returns>
Vector3 Perpendicular(const Vector3& v);

/// <summary>
///	クロス積
/// </summary>
/// <returns>計算結果</returns>
Vector3 Cross(const Vector3& v1, const Vector3& v2);

Vector3 Reflect(const Vector3& input, const Vector3& normal);