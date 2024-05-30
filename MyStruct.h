#pragma once
#include"Vector3.h"
#include<stdint.h>
struct Line{
	Vector3 origin;//始点
	Vector3 diff;//終点への線分ベクトル
};

struct Ray{
	Vector3 origin;
	Vector3 diff;
};

struct Segment{
	Vector3 origin;
	Vector3 diff;
	uint32_t color;
};