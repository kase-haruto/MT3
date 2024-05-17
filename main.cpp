#include<Novice.h>
#include<Matrix4x4.h>
#include<imgui.h>
#include"Sphere.h"
#include"Grid.h"
#include"MyFunc.h"
#include<memory>
#include"Camera.h"

const char kWindowTitle[] = "LE2A_09_カセ_ハルト_";

bool IsCollision(const Sphere* s1, const Sphere* s2){
	float distance = Vector3::Length(s1->GetCenter() - s2->GetCenter());
	if (distance <= s1->GetRadius() + s2->GetRadius()){
		return true;
	}
	return false;
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	std::unique_ptr<Camera> camera = std::make_unique<Camera>();

	Vector3 s1Center = {0.0f,0.0f,0.0f};
	Vector3 s2Center = {1.0f,0.0f,1.0f};
	Vector3 rotate = {0.0f,0.0f,0.0f};
	float radius = 0.5f;
	uint32_t color = WHITE;

	std::unique_ptr<Sphere> sphere1 = std::make_unique<Sphere>();
	sphere1->Init(s1Center, rotate, radius, color);

	std::unique_ptr<Sphere> sphere2 = std::make_unique<Sphere>();
	sphere2->Init(s2Center, rotate, radius, color);

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0){
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		//================================================================================================
		//		カメラの行列の計算
		//================================================================================================
		/*ImGui::Begin("camera");
		ImGui::DragFloat3("transtation", &cam.translate.x, 0.01f);
		ImGui::DragFloat3("rotation", &cam.rotate.x, 0.01f);
		ImGui::End();*/

		camera->Update();


		//================================================================================================
		//		球の更新
		//================================================================================================
		sphere1->UpdateImGui("sphere1");
		sphere2->UpdateImGui("sphere2");

		sphere1->SetColor(WHITE);
		sphere2->SetColor(WHITE);
		if (IsCollision(sphere1.get(), sphere2.get())){
			sphere1->SetColor(RED);
			sphere2->SetColor(RED);
		}

		//================================================================================================
		//		グリッドの描画
		//================================================================================================
		Grid::Draw(camera.get());


		//================================================================================================
		//		球の描画
		//================================================================================================
		sphere1->Draw(camera.get());
		sphere2->Draw(camera.get());

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
