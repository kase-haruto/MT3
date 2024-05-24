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

	Vector3 s1Center = {0.0f,0.0f,0.0f};
	Vector3 rotate = {0.0f,0.0f,0.0f};
	float radius = 0.5f;
	uint32_t color = WHITE;

	std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>();
	sphere->Init(s1Center, rotate, radius, color);

	std::unique_ptr<Plane> plane = std::make_unique<Plane>();
	plane->Initialze();

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
		sphere->UpdateImGui("sphere");
		plane->UpdateImGui();

		//================================================================================================
		//		カメラの行列の計算
		//================================================================================================
		camera->Update();

		//================================================================================================
		//		球の更新
		//================================================================================================		
		sphere->SetColor(WHITE);
		if (IsCollision(sphere.get(), plane.get())){
			sphere->SetColor(RED);
		}


		//================================================================================================
		//		グリッドの描画
		//================================================================================================
		Grid::Draw(camera);


		//================================================================================================
		//		球の描画
		//================================================================================================
		sphere->Draw(camera);

		//================================================================================================
		//		平面の描画
		//================================================================================================
		plane->Draw( camera, WHITE);

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
