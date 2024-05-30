#pragma once
#include"Sphere.h"
#include"Plane.h"
#include"MyStruct.h"

bool IsCollision(const Sphere* s, const Plane* plane);

bool IsCollision(const Segment& segment, const Plane* plane);