#include "Collision.h"
#include"MyFunc.h"

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
    Vector3 collisionPoint = segment.origin + segment.diff* t;

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
	if ((aabb1->GetMin().x <= aabb2->GetMax().x && aabb1->GetMax().x >= aabb2->GetMin().x)&&
		(aabb1->GetMin().y <= aabb2->GetMax().y && aabb1->GetMax().y >= aabb2->GetMin().y)&&
		(aabb1->GetMin().z <= aabb2->GetMax().z && aabb1->GetMax().z >= aabb2->GetMin().z)){
		return true;
	}
	return false;
}