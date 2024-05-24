#pragma once
#include"Vector3.h"
#include"Transform.h"
#include"Camera.h"
#include<stdint.h>

/// <summary>
/// 平面
/// </summary>
class Plane{
private:
	Vector3 normal_;//法線
	float distance_;//距離

	
public:
	/// <summary>
	/// コンストラクタ
	/// デストラクタ
	/// </summary>
	Plane();
	~Plane();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialze();
	/// <summary>
	/// 更新
	/// </summary>
	void UpdateImGui();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const Camera* camera, uint32_t color);

	const Vector3 GetNormal()const{ return normal_; }
	const float GetDistance()const{ return distance_; }
};

