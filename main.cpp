#include"Camera.h"
#include"MyFunc.h"
#include"MyStruct.h"
#include"Grid.h"
#include"Sphere.h"
#include"Transform.h"

#include<array>
#include<memory>
#include<Matrix4x4.h>
#include<Novice.h>

#ifdef _DEBUG
#include<imgui.h>
#endif // _DEBUG

const char kWindowTitle[] = "LE2A_09_カセ_ハルト_";



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Camera* camera = Camera::GetInstance();
	camera->Initialize();

	Vector3 a(0.2f, 1.0f, 0.0f);
	Vector3 b(2.4f, 3.1f, 1.2f);
	Vector3 c = a + b;
	Vector3 d = a - b;
	Vector3 e = a * 2.4f;
	Vector3 rotate {0.4f,1.43f,-0.8f};
	Matrix4x4 matRotateX = Matrix4x4::MakeRotateXMatrix(rotate.x);
	Matrix4x4 matRotateY = Matrix4x4::MakeRotateYMatrix(rotate.y);
	Matrix4x4 matRotateZ = Matrix4x4::MakeRotateZMatrix(rotate.z);
	Matrix4x4 matRotate = matRotateX * matRotateY * matRotateZ;


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0){
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		//================================================================================================
		//		imguiの更新
		//================================================================================================
		ImGui::Begin("window");
		ImGui::Text("c:%f,%f,%f", c.x, c.y, c.z);
		ImGui::Text("c:%f,%f,%f", d.x, d.y, d.z);
		ImGui::Text("c:%f,%f,%f", e.x, e.y, e.z);
		ImGui::Text(
			"matrix:\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n",
			matRotate.m[0][0], matRotate.m[0][1], matRotate.m[0][2],
			matRotate.m[0][3], matRotate.m[1][0], matRotate.m[1][1],
			matRotate.m[1][2], matRotate.m[1][3], matRotate.m[2][0],
			matRotate.m[2][1], matRotate.m[2][2], matRotate.m[2][3],
			matRotate.m[3][0], matRotate.m[3][1], matRotate.m[3][2],
			matRotate.m[3][3]);
		ImGui::End();


		//================================================================================================
		//		カメラの行列の計算
		//================================================================================================
		camera->Update();


		//================================================================================================
		//		グリッドの描画
		//================================================================================================
		Grid::Draw(camera);


		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0){
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
