#pragma once
#include"Sphere.h"
#include"Vector3.h"
#include"WorldTransform.h"

#include<memory>

class Ball final{
public:
	Ball();
	~Ball();

	void Initialize(const Vector3& center,const float& radius,const float & mass,const uint32_t& color);
	void Update();
	void Draw(Camera* cam);

public:
	Vector3 GetVelocity()const{ return velocity_; }
	Vector3 GetCenter()const{ return sphere_->GetCenter(); }
	Sphere* GetSphere()const{ return sphere_.get(); }
	float GetMass()const{ return mass_; }

	void SetVelocity(const Vector3& velocity){ velocity_ = velocity; }
	void SetCenter(const Vector3& center){ sphere_->SetCenter(center); }
	void SetMass(const float& mass){ mass_ = mass; }


private:
	std::unique_ptr<Sphere> sphere_ = nullptr;
	Vector3 velocity_;
	float mass_;
};

