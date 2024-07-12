#include"Sphere.h"
#include"Grid.h"
#include"Camera.h"
#include"MyFunc.h"
#include"Collision.h"
#include"MyStruct.h"

#include<memory>
#include<Matrix4x4.h>
#include<Novice.h>
#include<cmath>


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


	const Vector3 center = {0.0f,0.0f,0.0f};
	float radius = 0.8f;


	float angularVelocity = 3.14f;
	float angle = 0.0f;


	//重力
	const Vector3 kGravity {0.0f,-9.8f,0.0f};

	std::unique_ptr<Sphere>sphere = std::make_unique<Sphere>();
	sphere->Init({0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f}, 0.1f, WHITE);

	//bool isMove = false;
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
		//ImGui::Begin("window");
		//if (ImGui::Button("Start")){ isMove = true; }
		//ImGui::End();


		//================================================================================================
		//		カメラの行列の計算
		//================================================================================================
		camera->Update();


		//================================================================================================
		//		角度の計算
		//================================================================================================
		Vector3 pos;
		pos.x = center.x + radius * std::cos(angle);
		pos.y = center.y + radius * std::sin(angle);
		pos.z = 0.0f;
		angle += angularVelocity * deltaTime;
		sphere->SetCenter(pos);


		//================================================================================================
		//		グリッドの描画
		//================================================================================================
		Grid::Draw(camera);

		//================================================================================================
		//		円の描画
		//================================================================================================
		sphere->Draw(camera);


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
