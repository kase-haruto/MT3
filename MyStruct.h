#pragma once
#include"Vector3.h"
#include<stdint.h>
#include<cmath>

static const float deltaTime = 1.0f / 60.0f;
static const float gravity = 9.8f;

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

//振り子
struct Pendulum{
	Vector3 anchor;//アンカーポイント固定された端の位置
	float length;//ひもの長さ
	float angle;//現在の角度
	float angularVelocity;//速度
	float angularAcceleration;//加速度
	float mass;

	//振り子の先端位置を計算
	Vector3 TipPosition(){
		Vector3 pos;
		pos.x = anchor.x + std::sin(angle) * length;
		pos.y = anchor.y - std::cos(angle) * length;
		pos.z = anchor.z;
		return pos;
	}



};