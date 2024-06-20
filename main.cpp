#include"Sphere.h"
#include"Grid.h"
#include"Camera.h"
#include"MyFunc.h"
#include"Collision.h"
#include"MyStruct.h"

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

	std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>();
	sphere->Init({0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f}, 0.1f, WHITE);

	Pendulum pendulum;
	pendulum.anchor = {0.0f,1.0f,0.0f};
	pendulum.length = 0.8f;
	pendulum.angle = 0.7f;
	pendulum.angularVelocity = 0.0f;
	pendulum.angularAcceleration = -(gravity / pendulum.length) * std::sin(pendulum.angle);
	sphere->SetCenter(pendulum.TipPosition());
	
	bool isMove = false;

	

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
		ImGui::Begin("pendulum");
		if (ImGui::Button("start")){ isMove = true; }
		ImGui::End();

		//================================================================================================
		//		カメラの行列の計算
		//================================================================================================
		camera->Update();

		//================================================================================================
		//		振り子の計算
		//================================================================================================
		if (isMove){
			pendulum.angularAcceleration = -(gravity / pendulum.length) * std::sin(pendulum.angle);
			pendulum.angularVelocity += pendulum.angularAcceleration * deltaTime;
			pendulum.angle += pendulum.angularVelocity * deltaTime;
			sphere->SetCenter(pendulum.TipPosition());
		}
		

		//================================================================================================
		//		グリッドの描画
		//================================================================================================
		Grid::Draw(camera);

		//================================================================================================
		//		振り子の描画
		//================================================================================================
		
		//スクリーン座標まで変換させる
		Vector3 anchorNdcPos = Matrix4x4::Transform(pendulum.anchor, camera->GetViewProjection());
		Vector3 tipNdcPos = Matrix4x4::Transform(sphere->GetCenter(), camera->GetViewProjection());
		Vector3 anchorScreenPos = Matrix4x4::Transform(anchorNdcPos, camera->GetViewPort());
		Vector3 tipScreenPos = Matrix4x4::Transform(tipNdcPos, camera->GetViewPort());
		//ひもの描画
		Novice::DrawLine(static_cast< int >(anchorScreenPos.x), static_cast< int >(anchorScreenPos.y),
						 static_cast< int >(tipScreenPos.x), static_cast< int >(tipScreenPos.y),
						 WHITE);

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
