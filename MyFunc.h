#pragma once
#include"MyStruct.h"
#include"Camera.h"
#include<cmath>

#include<vector>

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

/// <summary>
/// 線形補完
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

void DrawBezier(const Vector3& ctrlPoint0, const Vector3& ctrolPoint1, const Vector3& ctrlPoint2,
				const Camera* camera, uint32_t color,bool drawPoints = false);

/// <summary>
/// CatmullRom補完
/// </summary>
/// <param name="p0"></param>
/// <param name="p1"></param>
/// <param name="p2"></param>
/// <param name="p3"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 CatmullRomInterpolation(
	const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);

Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t);

void DrawCatmullRom(const Vector3& ctrlP0,const Vector3& ctroP1,const Vector3& ctrlP2,
					const Vector3& ctrlP3,const Camera* camera,uint32_t color, bool drawPoints = false);