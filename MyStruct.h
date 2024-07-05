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

	Vector3 TipPosition(){
		Vector3 pos;
		pos.x = anchor.x + std::sin(angle) * length;
		pos.y = anchor.y - std::cos(angle) * length;
		pos.z = anchor.z;
		return pos;
	}
};

struct Spring{
	//アンカー、固定された端の位置
	Vector3 anchor;
	float naturalLength;//自然長
	float stiffness;//ばね定数
	float dampingCoefficient;//減衰係数
};

struct Ball{
	Vector3 position;	//ボールの位置
	Vector3 velocity;	//ボールの速度
	Vector3 acceleration;//ボールの加速度
	float mass;//ボールの質量
	float radius;//ボールの半径
	unsigned int color;//ボールの色
};
