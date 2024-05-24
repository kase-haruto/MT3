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