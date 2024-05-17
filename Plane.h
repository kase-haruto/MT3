#pragma once
#include"Vector3.h"
#include"Transform.h"
#include"Camera.h"

/// <summary>
/// 平面
/// </summary>
class Plane{
private:
	Transform wTransform_;
	Vector3 normal_ {1.0f,1.0f,1.0f};
	float distance_;

	
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
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const Camera* camera);
};

