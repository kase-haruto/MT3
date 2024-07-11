#include"MyFunc.h"
#include<Novice.h>
#include<cassert>
#include<algorithm>

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
	return Vector3(b.x * d, b.y * d, b.z * d);
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

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t){
	return Vector3(v1.x + (v2.x - v1.x) * t, v1.y + (v2.y - v1.y) * t, v1.z + (v2.z - v1.z) * t);
}

void DrawBezier(const Vector3& ctrlPoint0, const Vector3& ctrlPoint1, const Vector3& ctrlPoint2,
				const Camera* camera, uint32_t color, bool isDrawPoints){
	const size_t segmentCount = 30;

	//スクリーン座標に変換
	auto worldToScreen = [] (const Vector3& worldPos, const Camera* camera)->Vector3{
		Vector3 ndcPos = Matrix4x4::Transform(worldPos, camera->GetViewProjection());
		Vector3 screenPos = Matrix4x4::Transform(ndcPos, camera->GetViewPort());
		return screenPos;
		};

	for (size_t i = 0; i < segmentCount; ++i){
		float t1 = static_cast< float >(i) / segmentCount;
		float t2 = static_cast< float >(i + 1) / segmentCount;

		Vector3 p1 = Lerp(Lerp(ctrlPoint0, ctrlPoint1, t1), Lerp(ctrlPoint1, ctrlPoint2, t1), t1);
		Vector3 p2 = Lerp(Lerp(ctrlPoint0, ctrlPoint1, t2), Lerp(ctrlPoint1, ctrlPoint2, t2), t2);

		Vector3 screenPos1 = worldToScreen(p1, camera);
		Vector3 screenPos2 = worldToScreen(p2, camera);

		Novice::DrawLine(static_cast< int >(screenPos1.x),
						 static_cast< int >(screenPos1.y),
						 static_cast< int >(screenPos2.x),
						 static_cast< int >(screenPos2.y), color);
	}

	if (isDrawPoints){
		Vector3 ctrlPos = worldToScreen(ctrlPoint0, camera);
		Vector3 ctrlPos1 = worldToScreen(ctrlPoint1, camera);
		Vector3 ctrlPos2 = worldToScreen(ctrlPoint2, camera);
		int radius = 3;
		Novice::DrawEllipse(static_cast< int >(ctrlPos.x), static_cast< int >(ctrlPos.y), radius, radius, 0.0f, BLACK, kFillModeSolid);
		Novice::DrawEllipse(static_cast< int >(ctrlPos1.x), static_cast< int >(ctrlPos1.y), radius, radius, 0.0f, BLACK, kFillModeSolid);
		Novice::DrawEllipse(static_cast< int >(ctrlPos2.x), static_cast< int >(ctrlPos2.y), radius, radius, 0.0f, BLACK, kFillModeSolid);
	}
}

Vector3 CatmullRomInterpolation(
	const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t){

	float t2 = t * t;
	float t3 = t2 * t;

	float b0 = 0.5f * (-t3 + 2.0f * t2 - t);
	float b1 = 0.5f * (3.0f * t3 - 5.0f * t2 + 2.0f);
	float b2 = 0.5f * (-3.0f * t3 + 4.0f * t2 + t);
	float b3 = 0.5f * (t3 - t2);

	return Vector3(
		p0.x * b0 + p1.x * b1 + p2.x * b2 + p3.x * b3,
		p0.y * b0 + p1.y * b1 + p2.y * b2 + p3.y * b3,
		p0.z * b0 + p1.z * b1 + p2.z * b2 + p3.z * b3
	);
}

Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t){
	assert(points.size() >= 4 && "制御点は4以上が必要です");

	size_t segmentCount = points.size() - 1; // 区間数は制御点の数 - 3
	float areaWidth = 1.0f / segmentCount; // 各区間の長さ

	// 区間番号
	float t_2 = std::fmod(t, areaWidth) * segmentCount;
	t_2 = std::clamp(t_2, 0.0f, 1.0f);

	size_t index = static_cast< size_t >(t / areaWidth);
	index = std::clamp(int(index), 0, int(segmentCount - 1));

	// 4つの制御点のインデックス
	size_t index0 = index - 1;
	size_t index1 = index;
	size_t index2 = index + 1;
	size_t index3 = index + 2;

	// インデックスが範囲を超えないようにする
	index0 = std::clamp(index0, size_t(0), points.size() - 1);
	index1 = std::clamp(index1, size_t(0), points.size() - 1);
	index2 = std::clamp(index2, size_t(0), points.size() - 1);
	index3 = std::clamp(index3, size_t(0), points.size() - 1);

	const Vector3& p0 = points[index0];
	const Vector3& p1 = points[index1];
	const Vector3& p2 = points[index2];
	const Vector3& p3 = points[index3];

	return CatmullRomInterpolation(p0, p1, p2, p3, t_2);
}

void DrawCatmullRom(const Vector3& ctrlP0, const Vector3& ctrlP1, const Vector3& ctrlP2,
					const Vector3& ctrlP3, const Camera* camera, uint32_t color, bool drawPoints){
	const size_t segmentCount = 30;

	// スクリーン座標に変換
	auto worldToScreen = [] (const Vector3& worldPos, const Camera* camera) -> Vector3{
		Vector3 ndcPos = Matrix4x4::Transform(worldPos, camera->GetViewProjection());
		Vector3 screenPos = Matrix4x4::Transform(ndcPos, camera->GetViewPort());
		return screenPos;
		};

	std::vector<Vector3> points = {ctrlP0, ctrlP1, ctrlP2, ctrlP3};

	// Catmull-Romスプラインの各セグメントを描画
	for (size_t i = 0; i < segmentCount; ++i){
		float t1 = static_cast< float >(i) / segmentCount;
		float t2 = static_cast< float >(i + 1) / segmentCount;

		Vector3 p1 = CatmullRomPosition(points, t1);
		Vector3 p2 = CatmullRomPosition(points, t2);

		Vector3 screenPos1 = worldToScreen(p1, camera);
		Vector3 screenPos2 = worldToScreen(p2, camera);

		Novice::DrawLine(static_cast< int >(screenPos1.x),
						 static_cast< int >(screenPos1.y),
						 static_cast< int >(screenPos2.x),
						 static_cast< int >(screenPos2.y), color);
	}

	// 制御点を描画
	if (drawPoints){
		Vector3 ctrlPos0 = worldToScreen(ctrlP0, camera);
		Vector3 ctrlPos1 = worldToScreen(ctrlP1, camera);
		Vector3 ctrlPos2 = worldToScreen(ctrlP2, camera);
		Vector3 ctrlPos3 = worldToScreen(ctrlP3, camera);
		int radius = 3;
		Novice::DrawEllipse(static_cast< int >(ctrlPos0.x), static_cast< int >(ctrlPos0.y), radius, radius, 0.0f, BLACK, kFillModeSolid);
		Novice::DrawEllipse(static_cast< int >(ctrlPos1.x), static_cast< int >(ctrlPos1.y), radius, radius, 0.0f, BLACK, kFillModeSolid);
		Novice::DrawEllipse(static_cast< int >(ctrlPos2.x), static_cast< int >(ctrlPos2.y), radius, radius, 0.0f, BLACK, kFillModeSolid);
		Novice::DrawEllipse(static_cast< int >(ctrlPos3.x), static_cast< int >(ctrlPos3.y), radius, radius, 0.0f, BLACK, kFillModeSolid);
	}
}
