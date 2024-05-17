#pragma once
#include"Vector3.h"
#include"Matrix4x4.h"
#include"Transform.h"
#include<stdint.h>
#include"Camera.h"

class Sphere {

private://メンバ変数
	Transform transform_;
	float radius_;
	Matrix4x4 worldMatrix_;
	uint32_t color_;

private://メンバ関数
	

public:
	Sphere(){}
	virtual ~Sphere() {}

	void Init(Vector3 center, Vector3 rotate, float radius, uint32_t color);
	void UpdateImGui(const char* lavel);
	void Draw(const Camera* cam);
	
	///=================================
	///	アクセッサ
	///=================================
	
	void SetCenter(const Vector3& center);
	

	Vector3 GetRotate()const;
	void GetRotate(const Vector3& rotate);

	float GetRadius()const;
	void SetRadius(const float& radius);
	
	Matrix4x4 GetWorldMatrix()const;
	
	uint32_t GetColor()const;
	void SetColor(const uint32_t color);

	Vector3 GetCenter()const;

};

