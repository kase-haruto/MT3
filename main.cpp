#include <Novice.h>
#include<cmath>
#include"Matrix4x4.h"
#include"Vector3.h"
#include"Vector4.h"
#include<vector>
#include<stdint.h>
#include<assert.h>

const char kWindowTitle[] = "LE2A_09_カセ_ハルト_";

const int kWindowHeight = 720;
const int kWindowWidth = 1240;

const int kRowHeight = 20;
const int kColumnWidth = 60;

Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result = { 0, 0, 0 };

	// 同次座標系への変換
	// 変換行列を適用
	Vector4 homogeneousCoordinate(
		vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0],
		vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1],
		vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2],
		vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[3][3]
	);

	// 同次座標系から3次元座標系に戻す
	float w = homogeneousCoordinate.w;
	assert(w != 0.0f); // wが0でないことを確認
	result.x = homogeneousCoordinate.x / w;
	result.y = homogeneousCoordinate.y / w;
	result.z = homogeneousCoordinate.z / w;

	return result;
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 cameraPosition{ 0.0f,0.0f,0.0f };

	Vector3 rotate{ 0.0f,0.0f,0.0f };
	Vector3 translate{ 0.0f,0.0f,200.0f };

	Matrix4x4 worldMatrix;

	Matrix4x4 cameraMatrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 worldViewProjectionMatrix;
	Matrix4x4 viewportMatrix;

	Vector3 screenVertices[3];


	Vector3 kLocalVertices[3] = {
		{0.0f,25.0f,0.0f},
		{25.0f,-25.0f,0.0f},
		{-25.0f,-25.0f,0.0f}
	};


	Vector3 v1{ 1.2f,-3.9f,2.5f };
	Vector3 v2{ 2.8f,0.4f,-1.3f };
	Vector3 cross = Cross(v1, v2);


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		//自動でY軸で回転する
		rotate.y += 0.05f;

		//キーで三角形の移動を行う
		if (keys[DIK_W]) {
			translate.z += 10.0f;
		} else if (keys[DIK_S]) {
			translate.z -= 10.0f;
		}

		if (keys[DIK_A]) {
			translate.x -= 10.0f;
		} else if (keys[DIK_D]) {
			translate.x += 10.0f;
		}


		// 各行列の計算
		worldMatrix = Matrix4x4::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, rotate, translate);
		cameraMatrix = Matrix4x4::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, cameraPosition);
		viewMatrix = Matrix4x4::Inverse(cameraMatrix);
		projectionMatrix = Matrix4x4::MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		//wvpMatrixを作成
		worldViewProjectionMatrix = Matrix4x4::Multiply(worldMatrix, Matrix4x4::Multiply(viewMatrix, projectionMatrix));
		//viewportMatrixを作成
		viewportMatrix = Matrix4x4::MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		for (uint32_t i = 0; i < 3; ++i) {
			Vector3 ndcVertex = Transform(kLocalVertices[i], worldViewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}



		//クロス積の表示
		Vector3::PrintVector3(0, 0, cross, "cross");

		// 三角形の描画
		Novice::DrawTriangle(
			int(screenVertices[0].x), int(screenVertices[0].y),
			int(screenVertices[1].x), int(screenVertices[1].y),
			int(screenVertices[2].x), int(screenVertices[2].y),
			RED, kFillModeSolid
		);

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
