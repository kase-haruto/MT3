#pragma once
#include"Vector3.h"
#include"Matrix4x4.h"

/// <summary>
/// カメラクラス
/// </summary>
class Camera{
private:
	Vector3 translate_ = {0.0f,1.9f,-6.49f};
	Vector3 rotate_ = { 0.26f, 0.0f, 0.0f };

	Matrix4x4 cameraMatrix_;
	Matrix4x4 matView_;
	Matrix4x4 matProjection_;
	Matrix4x4 matViewProjection_;
	Matrix4x4 matViewPort_;

public:
	Camera();
	~Camera();

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static Camera* GetInstance();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	Matrix4x4 GetViewPort()const{ return matViewPort_; }
	Matrix4x4 GetViewProjection()const{ return matViewProjection_; }
	
private:
	Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
	Matrix4x4 MakeViewportMatrix(float l, float t, float w, float h, float minDepth, float maxDepth);

	
	// コピー禁止
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;
};

