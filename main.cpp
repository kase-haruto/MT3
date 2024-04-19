#include<Novice.h>
#include<Matrix4x4.h>
#include<imgui.h>
#include"Sphere.h"
#include"Grid.h"

const char kWindowTitle[] = "LE2A_09_カセ_ハルト_";

/// <summary>
/// 2つのベクトルの外積を計算します
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns>外積の結果</returns>
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

/// <summary>
/// 2つのベクトルの内積を計算します。
/// </summary>
/// <param name="v1">ベクトル1</param>
/// <param name="v2">ベクトル2</param>
/// <returns>内積の結果</returns>
float DotProduct(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}


struct Camera {
	Vector3 translate;
	Vector3 rotate;
	Vector3 direction;
	Matrix4x4 cameraMatrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 worldViewProjectionMatrix;
	Matrix4x4 viewportMatrix;

	Vector3 GetDirection();
};

Vector3 Camera::GetDirection() {
	return Vector3(viewMatrix.m[0][2], viewMatrix.m[1][2], viewMatrix.m[2][2]);
}



/*==================================================================================================
		グリッド線を引く関数
===================================================================================================*/


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Camera cam;
	cam.translate = { 0.0f,1.9f,-6.49f };
	cam.rotate = { 0.26f,0.0f,0.0f };
	cam.projectionMatrix = Matrix4x4::MakePerspectiveFovMatrix(0.45f, 1280.0f / 720.0f, 0.1f, 100.0f);
	cam.viewportMatrix = Matrix4x4::MakeViewportMatrix(0, 0, 1280.0f, 720.0f, 0.0f, 1.0f);

	Sphere* sphere = new Sphere();
	sphere->Init({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, 0.5f, BLACK);

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		ImGui::Begin("camera");
		ImGui::DragFloat3("translate", &cam.translate.x, 0.01f);
		ImGui::DragFloat3("rotate", &cam.rotate.x, 0.01f);
		ImGui::End();

		//================================================================================================
		//		カメラの行列の計算
		//================================================================================================
		cam.cameraMatrix = Matrix4x4::MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cam.rotate, cam.translate);
		cam.viewMatrix = Matrix4x4::Inverse(cam.cameraMatrix);
		Matrix4x4 viewProjectionMatrix = Matrix4x4::Multiply(cam.viewMatrix, cam.projectionMatrix);

		sphere->Update();

		//================================================================================================
		//		グリッドの描画
		//================================================================================================
		Grid::Draw(viewProjectionMatrix, cam.viewportMatrix);

		//================================================================================================
		//		球体
		//================================================================================================

		sphere->Draw(viewProjectionMatrix, cam.viewportMatrix);




		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	delete sphere;

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
