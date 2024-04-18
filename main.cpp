#include <Novice.h>
#include<cmath>
#include"Matrix4x4.h"

const char kWindowTitle[] = "LC1B_09_カセ_ハルト_";

Matrix4x4 MakerotateXMatrix(float theta) {
	Matrix4x4 result = {
		1,0,0,0,
		0,std::cos(theta),std::sin(theta),0,
		0,-std::sin(theta),std::cos(theta),0,
		0,0,0,1
	};

	return result;
}

Matrix4x4 MakerotateYMatrix(float theta) {
	Matrix4x4 result = {
		std::cos(theta),0,-std::sin(theta),0,
		0,1,0,0,
		std::sin(theta),0,std::cos(theta),0,
		0,0,0,1
	};
	return result;
}

Matrix4x4 MakerotateZMatrix(float theta) {
	Matrix4x4 result = {
		std::cos(theta),std::sin(theta),0,0,
		-std::sin(theta),std::cos(theta),0,0,
		0,0,1,0,
		0,0,0,1
	};
	return result;
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 rotate{ 0.4f,1.43f,-0.8f };
	Matrix4x4 rotateXMatrix = MakerotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakerotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakerotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZMatrix = Matrix4x4::Multiply(rotateXMatrix, Matrix4x4::Multiply(rotateYMatrix, rotateZMatrix));


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		Matrix4x4::MatrixScreenPrint(0, 0, rotateXMatrix, "rotateXMatrix");
		Matrix4x4::MatrixScreenPrint(0, 20 * 5, rotateYMatrix, "rotateYMatrix");
		Matrix4x4::MatrixScreenPrint(0, 20 * 5 * 2, rotateZMatrix, "rotateZMatrix");
		Matrix4x4::MatrixScreenPrint(0, 20 * 5 * 3, rotateXYZMatrix, "rotateXYZMatrix");

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
