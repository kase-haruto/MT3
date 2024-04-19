#pragma once
#include"Vector3.h"
#include"Matrix4x4.h"
#include"Transform.h"
#include<stdint.h>

class Sphere {

private://メンバ変数
	Transform transform_;
	float radius_;
	Matrix4x4 worldMatrix_;
	uint32_t color_;

private://メンバ関数
	

public:
	Sphere(Vector3 center, float radius, uint32_t color) { transform_.translation = center; radius_ = radius; color_ = color; }
	virtual ~Sphere() {}

	void Init(Vector3 center, Vector3 rotate, float radius, uint32_t color);
	void Update();
	void Draw(Matrix4x4& viewProjection, Matrix4x4& viewPort);


	Vector3 GetCenter()const;
	void SetCenter(const Vector3& center);

	Vector3 GetRotate()const;
	void GetRotate(const Vector3& rotate);

	float GetRadius()const;
	void SetRadius(const float& radius);
	
	Matrix4x4 GetWorldMatrix()const;
	
	uint32_t GetColor()const;
	void SetColor(const uint32_t color);

};

