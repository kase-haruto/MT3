﻿#include "Collision.h"
#include"MyFunc.h"
#include<cmath>
#include<algorithm>

bool IsCollision(const Sphere* s, const Plane* plane){
	Vector3 spherePos = s->GetCenter();
	float sphereRadius = s->GetRadius();
	Vector3 planeNormal = plane->GetNormal();
	float planeDistance = plane->GetDistance();
	Vector3 pos = planeNormal * planeDistance;
	float distance;

	float d = Dot(pos, planeNormal);

	distance = std::abs(Dot(planeNormal, spherePos) - d);

	if (distance <= sphereRadius){
		return true;
	}
	return false;
}

bool IsCollision(const Segment& segment, const Plane* plane){
	float planeDis = plane->GetDistance();
	Vector3 planeNormal = plane->GetNormal();
	Vector3 n = Vector3::Normalize(planeNormal);
	float d = Dot(n, segment.diff);

	// 平行だから衝突しない
	if (d == 0.0f){
		return false;
	}

	// tを求める
	float t = (planeDis - Dot(segment.origin, n)) / d;

	// tが0から1の範囲にある場合、衝突している
	if (t >= 0.0f && t <= 1.0f){
		return true;
	}

	return false;
}

bool IsCollision(const Segment& segment, const Triangle* triangle){
	// 三角形の頂点を取得
	const Vector3& v0 = triangle->GetTop();
	const Vector3& v1 = triangle->GetRBottom();
	const Vector3& v2 = triangle->GetLBottom();

	// 三角形の法線を計算
	Vector3 edge1 = v1 - v0;
	Vector3 edge2 = v2 - v0;
	Vector3 normal = Vector3::Normalize(Cross(edge1, edge2));

	// 平面の距離を計算
	float d = Dot(normal, v0);

	// 平面との衝突点を求める
	Vector3 n = Vector3::Normalize(normal);
	float denominator = Dot(n, segment.diff);

	float t = (d - Dot(segment.origin, n)) / denominator;

	if (t < 0.0f || t > 1.0f){
		return false;
	}

	// 衝突点を計算
	Vector3 collisionPoint = segment.origin + segment.diff * t;

	// 三角形の各辺と衝突点を結んだベクトルのクロス積を計算
	Vector3 cross0 = Cross(v1 - v0, collisionPoint - v0);
	Vector3 cross1 = Cross(v2 - v1, collisionPoint - v1);
	Vector3 cross2 = Cross(v0 - v2, collisionPoint - v2);

	// クロス積と法線のドット積をチェック
	if (Dot(cross0, normal) >= 0.0f && Dot(cross1, normal) >= 0.0f && Dot(cross2, normal) >= 0.0f){
		return true;
	}

	return false;
}

bool isCollision(const AABB* aabb1, const AABB* aabb2){
	if ((aabb1->GetMin().x <= aabb2->GetMax().x && aabb1->GetMax().x >= aabb2->GetMin().x) &&
		(aabb1->GetMin().y <= aabb2->GetMax().y && aabb1->GetMax().y >= aabb2->GetMin().y) &&
		(aabb1->GetMin().z <= aabb2->GetMax().z && aabb1->GetMax().z >= aabb2->GetMin().z)){
		return true;
	}
	return false;
}

bool isCollision(const Sphere* s, const AABB* aabb){
	Vector3 closestPoint {
		std::clamp(s->GetCenter().x,aabb->GetMin().x,aabb->GetMax().x),
		std::clamp(s->GetCenter().y,aabb->GetMin().y,aabb->GetMax().y),
		std::clamp(s->GetCenter().z,aabb->GetMin().z,aabb->GetMax().z),
	};

	//距離を求める
	float distance = Vector3::Length(closestPoint - s->GetCenter());

	if (distance <= s->GetRadius()){
		return true;
	}
	return false;

}

bool isCollision(const Segment& segment, const AABB* aabb){
	Vector3 t_min = (aabb->GetMin() - segment.origin) / segment.diff;
	Vector3 t_max = (aabb->GetMax() - segment.origin) / segment.diff;

	Vector3 tNear = Vector3::Min(t_min, t_max);
	Vector3 tFar = Vector3::Max(t_min, t_max);

	float tMinF = std::max(std::max(tNear.x, tNear.y), tNear.z);
	float tMaxF = std::min(std::min(tFar.x, tFar.y), tFar.z);

	if (tMaxF >= 0.0f && tMinF <= 1.0f && tMaxF >= tMinF){
		return true;
	}
	return false;
}

bool IsCollision(const OBB* obb, const Sphere* sphere){
	// 球の中心
	Vector3 sphereCenter = sphere->GetCenter();

	// OBBの中心
	Vector3 obbCenter = obb->GetCenter();

	// 球の中心をOBBのローカル座標系に変換
	Vector3 localSphereCenter = sphereCenter - obbCenter;

	// 球の中心をOBBの軸に投影
	Vector3 closestPointInOBB;
	float distance;
	Vector3 obbOrientation[3];
	Vector3 obbSize = obb->GetSize(); // OBBのサイズを取得
	for (int i = 0; i < 3; ++i){
		obbOrientation[i] = obb->GetOrientation()[i];
		distance = Dot(localSphereCenter, obbOrientation[i]);
		// OBBの各軸方向における最近接点を求める
		if (i == 0){
			if (distance > obbSize.x) distance = obbSize.x;
			if (distance < -obbSize.x) distance = -obbSize.x;
		} else if (i == 1){
			if (distance > obbSize.y) distance = obbSize.y;
			if (distance < -obbSize.y) distance = -obbSize.y;
		} else if (i == 2){
			if (distance > obbSize.z) distance = obbSize.z;
			if (distance < -obbSize.z) distance = -obbSize.z;
		}
		closestPointInOBB += distance * obbOrientation[i];
	}
	// OBBの最も近い点から球の中心までのベクトル
	Vector3 vectorFromOBB = localSphereCenter - closestPointInOBB;

	// 距離の平方を計算
	float distanceSquared = Dot(vectorFromOBB, vectorFromOBB);

	// 球の半径の平方
	float radiusSquared = sphere->GetRadius() * sphere->GetRadius();

	// 距離の平方が球の半径の平方以下であれば衝突
	if (distanceSquared <= radiusSquared){
		return true;
	}
	return false;
}

