#include "Collision.h"
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

bool isCollision(const Sphere* s, const AABB* aabb){
	Vector3 closestPoint {
		std::clamp(s->GetCenter().x,aabb->GetMin().x,aabb->GetMax().x),
		std::clamp(s->GetCenter().y,aabb->GetMin().y,aabb->GetMax().y),
		std::clamp(s->GetCenter().z,aabb->GetMin().z,aabb->GetMax().z),
	};

	//距離を求める
	float distance = Vector3::Length(closestPoint - s->GetCenter());

	if (distance<=s->GetRadius()){
		return true;
	}
	return false;

}

bool isCollision(const Segment& segment, const AABB* aabb){
	Vector3 invDir = Vector3(1.0f / segment.diff.x, 1.0f / segment.diff.y, 1.0f / segment.diff.z);

	Vector3 t1 = (aabb->GetMin() - segment.origin) * invDir;
	Vector3 t2 = (aabb->GetMax() - segment.origin) * invDir;

	Vector3 tNear = Vector3::Min(t1, t2);
	Vector3 tFar = Vector3::Max(t1, t2);

	float tMinF = std::max(std::max(tNear.x, tNear.y), tNear.z);
	float tMaxF = std::min(std::min(tFar.x, tFar.y), tFar.z);

	return tMaxF >= tMinF && tMaxF >= 0.0f && tMinF <= 1.0f;
}

bool IsCollision(Sphere* s1, Sphere* s2){
	float distance = Vector3::Length(s1->GetCenter() - s2->GetCenter());
	if (distance <= s1->GetRadius() + s2->GetRadius()){
		return true;
	}
	return false;


	//Vector3 s1Pos = s1->GetCenter();
	//Vector3 s1PrePos = s1->GetPreCenter();
	//float s1Radius = s1->GetRadius();
	//Vector3 s2Pos = s2->GetCenter();
	//Vector3 s2PrePos = s2->GetPreCenter();
	//float s2Radius = s2->GetRadius();

	//// s1からs2への方向ベクトル（現在の位置）
	//Vector3 directionS1ToS2 = s2Pos - s1Pos;
	//// s1の前の位置から現在の位置へのベクトル
	//Vector3 directionS1 = s1Pos - s1PrePos;

	//// 正規化
	//Vector3 normalizedDir = Vector3::Normalize(directionS1);
	//// tの値を求める
	//float t = Dot(directionS1ToS2, normalizedDir) / Vector3::Length(directionS1);
	//// tを0~1の範囲に収める
	//t = std::clamp(t, 0.0f, 1.0f);

	//// 線形補完
	//Vector3 interpolatedPos = s1PrePos + directionS1 * t;

	//// s2の中心位置との距離を求める
	//float distance = Vector3::Length(s2Pos - interpolatedPos);

	//return distance < (s1Radius + s2Radius);
}

float ClosestPointsBetweenLines(
    const Vector3& p1, const Vector3& q1,
    const Vector3& p2, const Vector3& q2,
    Vector3& c1, Vector3& c2){
    Vector3 d1 = q1 - p1; // 線分1の方向ベクトル
    Vector3 d2 = q2 - p2; // 線分2の方向ベクトル
    Vector3 r = p1 - p2;
    float a = Dot(d1, d1); // d1の長さの2乗
    float e = Dot(d2, d2); // d2の長さの2乗
    float f = Dot(d2, r);

    float s, t;
    if (a <= std::numeric_limits<float>::epsilon() && e <= std::numeric_limits<float>::epsilon()){
        // 両方の線分が点の場合
        c1 = p1;
        c2 = p2;
        return (c1 - c2).LengthSquared();
    }
    if (a <= std::numeric_limits<float>::epsilon()){
        // 線分1が点の場合
        s = 0.0f;
        t = f / e;
        t = std::clamp(t, 0.0f, 1.0f);
    } else{
        float c = Dot(d1, r);
        if (e <= std::numeric_limits<float>::epsilon()){
            // 線分2が点の場合
            t = 0.0f;
            s = std::clamp(-c / a, 0.0f, 1.0f);
        } else{
            float b = Dot(d1, d2);
            float denom = a * e - b * b;
            if (denom != 0.0f){
                s = std::clamp((b * f - c * e) / denom, 0.0f, 1.0f);
            } else{
                s = 0.0f;
            }
            t = (b * s + f) / e;
            if (t < 0.0f){
                t = 0.0f;
                s = std::clamp(-c / a, 0.0f, 1.0f);
            } else if (t > 1.0f){
                t = 1.0f;
                s = std::clamp((b - c) / a, 0.0f, 1.0f);
            }
        }
    }

    c1 = p1 + d1 * s;
    c2 = p2 + d2 * t;
    return (c1 - c2).LengthSquared();
}
