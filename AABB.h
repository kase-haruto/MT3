#pragma once
#include"Vector3.h"
#include"Camera.h"
#include<stdint.h>
#include<string>
class AABB{
private:
	Vector3 min_;
	Vector3 max_;
	uint32_t color_;

public:
	AABB();
	~AABB();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <param name="color"></param>
	void Initialize(Vector3 min,Vector3 max,uint32_t color);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cam"></param>
	void Draw(Camera* cam);
	/// <summary>
	/// uiの更新
	/// </summary>
	void UpdateUI(std::string lavel);

	Vector3 GetMin()const;
	Vector3 GetMax()const;

	void SetMin(const Vector3 min);
	void SetMax(const Vector3 max);

	uint32_t GetColor()const;
	void SetColor(const uint32_t color);
};

