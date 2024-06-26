#pragma once
#include"Sphere.h"
#include"Plane.h"
#include"Triangle.h"
#include"MyStruct.h"
#include"AABB.h"

bool IsCollision(const Sphere* s, const Plane* plane);

bool IsCollision(const Segment& segment, const Plane* plane);

bool IsCollision(const Segment& segment, const Triangle* triangle);

bool isCollision(const AABB* aabb1, const AABB* aabb2);

bool isCollision(const Sphere* s, const AABB* aabb);

bool isCollision(const Segment& segment, const AABB* aabb);

bool IsCollision(Sphere* s1,Sphere* s2);

float ClosestPointsBetweenLines(const Vector3& p1, const Vector3& q1,
                                const Vector3& p2, const Vector3& q2,
                                Vector3& c1, Vector3& c2);