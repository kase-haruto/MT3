#pragma once
#include"Matrix4x4.h"
#include"Camera.h"
struct Grid {
	
	/// <summary>
	/// グリッドを描画します
	/// </summary>
	/// <param name="viewProjectionMatrix"></param>
	/// <param name="viewportMatrix"></param>
	/// <param name="startPos"></param>
	/// <param name="endPos"></param>
	static void DrawGridLine(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, const Vector3& startPos, const Vector3& endPos);
	
	/// <summary>
	/// 縦横の線を描画します
	/// </summary>
	/// <param name="viewProjectionMatrix"></param>
	/// <param name="viewportMatrix"></param>
	static void Draw(const Camera* cam,const float widht = 2.0f);
};

