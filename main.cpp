#include <Novice.h>
#include<assert.h>
#include "Matrix4x4.h"
#include"Vector3.h"
#include"Vector4.h"

const char kWindowTitle[] = "LE2A_09_カセ_ハルト";

//平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		translate.x,translate.y,translate.z,1
	};
	return result;
}

//拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result = {
		scale.x,0,0,0,
		0,scale.y,0,0,
		0,0,scale.z,0,
		0,0,0,1
	};
	return result;
}

//座標変換
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

const int kColumWidth = 60;
const int kRowHeight = 60;

void PrintVector3(int x, int y, const Vector3& v, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", v.x);
	Novice::ScreenPrintf(x + kColumWidth, y, "%.02f", v.y);
	Novice::ScreenPrintf(x + kColumWidth * 2, y, "%.02f", v.z);
	Novice::ScreenPrintf(x + kColumWidth * 3, y, "%s", label);
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 translate{ 4.1f,2.6f,0.8f };
	Vector3 scale{ 1.5f,5.2f,7.3f };
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Vector3 point{ 2.3f,3.8f,1.4f };
	Matrix4x4 transformMatrix = {
		1.0f,2.0f,3.0f,4.0f,
		3.0f,1.0f,1.0f,2.0f,
		1.0f,4.0f,2.0f,3.0f,
		2.0f,2.0f,1.0f,3.0f
	};

	Vector3 transfomed = Transform(point, transformMatrix);


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		PrintVector3(0, 0, transfomed, "transformed");
		Matrix4x4::MatrixScreenPrint(0, 20, translateMatrix, "translateMatrix");
		Matrix4x4::MatrixScreenPrint(0, 30 * 5, scaleMatrix, "scaleMatrix");

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
