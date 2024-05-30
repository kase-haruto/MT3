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
