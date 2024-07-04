#include"Camera.h"
#include"MyFunc.h"
#include"MyStruct.h"
#include"Grid.h"

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

	Vector3 ctrlPoints[3] = {
		{-0.8f,0.58f,1.0f},
		{1.76f,1.0f,-0.3f},
		{0.94f,-0.7f,2.3f}
	};


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
		ImGui::DragFloat3("ctrlP0", &ctrlPoints[0].x, 0.01f);
		ImGui::DragFloat3("ctrlP1", &ctrlPoints[1].x, 0.01f);
		ImGui::DragFloat3("ctrlP2", &ctrlPoints[2].x, 0.01f);
		ImGui::End();


		//================================================================================================
		//		カメラの行列の計算
		//================================================================================================
		camera->Update();


		//================================================================================================
		//		グリッドの描画
		//================================================================================================
		Grid::Draw(camera);


		//================================================================================================
		//		ベジエ曲線の描画
		//================================================================================================
		DrawBezier(ctrlPoints[0], ctrlPoints[1], ctrlPoints[2],camera, WHITE,true);


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
