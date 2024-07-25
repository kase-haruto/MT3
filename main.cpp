#include"Sphere.h"
#include"Grid.h"
#include"Camera.h"
#include"MyFunc.h"
#include"Collision.h"
#include"MyStruct.h"
#include"Plane.h"
#include"PhysicsSphere.h"

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


	std::unique_ptr<Plane>plane = std::make_unique<Plane>();
	Vector3 normal = Vector3::Normalize({-0.2f,0.9f,-0.3f});
	plane->SetNormal(normal);
	plane->SetDistance(0.0f);

	std::unique_ptr< PhysicsSphere> ball = std::make_unique<PhysicsSphere>();
	Vector3 ballCenter = {0.8f,1.2f,0.3f};
	float mass = 2.0f;
	Vector3 acceleration = {0.0f,-gravity,0.0f};
	ball->Initialize(ballCenter,0.05f,WHITE);
	ball->SetAcceleration(acceleration);
	ball->SetMass(mass);

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
		plane->UpdateImGui();
		ball->Debug();

		ImGui::Begin("setting");
		if (ImGui::Button("isMove")){isMove = !isMove;}
		ImGui::End();

		//================================================================================================
		//		カメラの行列の計算
		//================================================================================================
		camera->Update();

		//================================================================================================
		//		ballの計算
		//================================================================================================
		if (isMove){
			ball->Update();
			//画面外でリセット
			if (ball->GetCenter().y <= -2){
				ball->SetCenter(ballCenter);
				ball->SetVelocity({0.0f,0.0f,0.0f});
				ball->SetAcceleration(acceleration);
				isMove = false;
			}
			
		}

		if (IsCollision(ball.get(), plane.get())){
			Vector3 reflected = Reflect(ball->GetVelocity(), plane->GetNormal());
			Vector3 projectToNormal = Projection(reflected, plane->GetNormal());
			Vector3 movingDirection = reflected - projectToNormal;
			ball->SetVelocity(projectToNormal *0.7f + movingDirection);
		}
		

		//================================================================================================
		//		グリッドの描画
		//================================================================================================
		Grid::Draw(camera);

		//================================================================================================
		//		平面
		//================================================================================================
		plane->Draw(camera, WHITE);
	
		//================================================================================================
		//		球体
		//================================================================================================
		ball->Draw(camera);



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
