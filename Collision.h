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