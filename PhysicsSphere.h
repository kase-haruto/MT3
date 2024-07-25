#pragma once
#include"Sphere.h"

/// <summary>
/// 物理の動きを追加した球体
/// </summary>
class PhysicsSphere final:
	public Sphere{

public:
	PhysicsSphere();
	~PhysicsSphere();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="center"></param>
	/// <param name="radius"></param>
	/// <param name="color"></param>
	void Initialize(Vector3 center,float radius, uint32_t color);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(Camera* camera);

#ifdef _DEBUG
	void Debug();
#endif // DEBUG


	Vector3 GetVelocity()const{ return velocity_; }
	Vector3 GetAcceleration()const{ return acceleration_; }
	float GetMass()const{ return mass_; }	
	
	void SetVelocity(const Vector3& velocity){ velocity_ = velocity; }
	void SetAcceleration(const Vector3& acceleration){ acceleration_ = acceleration; }
	void SetMass(const float mass){ mass_ = mass; }

private:
	Vector3 velocity_;
	Vector3 acceleration_;
	float mass_;
};

