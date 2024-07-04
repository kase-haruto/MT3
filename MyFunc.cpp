#include"MyFunc.h"
#include<Novice.h>

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
				const Camera* camera, uint32_t color,bool isDrawPoints){
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

		Vector3 screenPos1 = worldToScreen(p1,camera);
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