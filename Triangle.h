#pragma once
#include"Vector3.h"
#include"Camera.h"
#include<stdint.h>

class Triangle{
private:
	Vector3 vertices_[3] = {{-1.0f,0.0f,0.0f},{0.0f,1.0f,0.0f} ,{ 1.0f,0.0f,0.0f }};
	uint32_t color_;

public:
	Triangle();
	~Triangle();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(uint32_t color = 0xffffffff);
	/// <summary>
	/// uiの更新
	/// </summary>
	void UiUpdate();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(Camera* camera);

	Vector3 GetTop()const;
	Vector3 GetRBottom()const;
	Vector3 GetLBottom()const;
	void SetTop(const Vector3 vertices);
	void SetRBottom(const Vector3 vertices);
	void SetLBottom(const Vector3 vertices);
};

