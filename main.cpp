#include<Novice.h>
#include<Matrix4x4.h>
#include<imgui.h>
#include"Sphere.h"
#include"Grid.h"
#include<memory>
#include"Camera.h"
#include"MyFunc.h"
#include"Plane.h"
#include"MyFunc.h"
#include"Collision.h"
#include"MyStruct.h"
#include"AABB.h"
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

	std::unique_ptr<AABB> aabb1 = std::make_unique<AABB>();
	aabb1->Initialize({-0.5f,-0.5f,-0.5f} ,{0.0f,0.0f,0.0f},WHITE);

	std::unique_ptr<AABB> aabb2 = std::make_unique<AABB>();
	aabb2->Initialize({0.2f,0.2f,0.2f}, {1.0f,1.0f,1.0f}, WHITE);

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
		aabb1->UpdateUI("aabb1");
		aabb2->UpdateUI("aabb2");
		//================================================================================================
		//		カメラの行列の計算
		//================================================================================================
		camera->Update();

		//================================================================================================
		//		aabbの更新
		//================================================================================================
		aabb1->Update();
		aabb2->Update();

		//================================================================================================
		//		衝突判定
		//================================================================================================
		aabb1->SetColor(WHITE);
		aabb2->SetColor(WHITE);
		if (isCollision(aabb1.get(), aabb2.get())){
			aabb1->SetColor(RED);
			aabb2->SetColor(RED);
		}

		//================================================================================================
		//		グリッドの描画
		//================================================================================================
		Grid::Draw(camera);

		//================================================================================================
		//		aabbの描画
		//================================================================================================
		aabb1->Draw(camera);
		aabb2->Draw(camera);
		
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
