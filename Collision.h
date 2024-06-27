#pragma once
#include"Sphere.h"
#include"Plane.h"
#include"Triangle.h"
#include"MyStruct.h"
#include"AABB.h"
#include"OBB.h"

bool IsCollision(const Sphere* s, const Plane* plane);

bool IsCollision(const Segment& segment, const Plane* plane);

bool IsCollision(const Segment& segment, const Triangle* triangle);

bool isCollisionAABB(const AABB& aabb1, const AABB& aabb2);

bool isCollision(const Sphere* s, const AABB* aabb);

bool isCollision(const Segment& segment, const AABB* aabb);

bool IsCollision(const OBB* obb, const Sphere* sphere);

bool IsCollision(const OBB* obb1, const OBB* obb2);