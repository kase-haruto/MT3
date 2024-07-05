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

	

	Spring spring {};
	spring.anchor = {0.0f,0.0f,0.0f};
	spring.naturalLength = 1.0f;
	spring.stiffness = 100.0f;
	spring.dampingCoefficient = 2.0f;

	Ball ball {};
	ball.position = {1.2f,0.0f,0.0f};
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.color = BLUE;

	std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>();
	sphere->Init(ball.position, {0.0f,0.0f,0.0f}, ball.radius, ball.color);


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

		ImGui::End();

		//================================================================================================
		//		カメラの行列の計算
		//================================================================================================
		camera->Update();

		//================================================================================================
		//		ばねの計算
		//================================================================================================
		Vector3 diff = ball.position - spring.anchor;
		float length = Vector3::Length(diff);
		if (length != 0.0f){
			Vector3 direction = Vector3::Normalize(diff);
			Vector3 restPosition = spring.anchor + direction * spring.naturalLength;

			Vector3 displacement;
			displacement.x= length * (ball.position.x - restPosition.x);
			displacement.y= length * (ball.position.y - restPosition.y);
			displacement.z= length * (ball.position.z - restPosition.z);

			Vector3 restoringForce;
			restoringForce.x = -spring.stiffness * displacement.x;
			restoringForce.y = -spring.stiffness * displacement.y;
			restoringForce.z = -spring.stiffness * displacement.z;

			//減衰抵抗を計算する
			Vector3 dampingForce;
			dampingForce.x = -spring.dampingCoefficient * ball.velocity.x;
			dampingForce.y = -spring.dampingCoefficient * ball.velocity.y;
			dampingForce.z = -spring.dampingCoefficient * ball.velocity.z;

			Vector3 force = restoringForce + dampingForce;
			ball.acceleration = force / ball.mass;
		}

		ball.velocity += ball.acceleration * deltaTime;
		ball.position += ball.velocity * deltaTime;
		sphere->SetCenter(ball.position);

		//================================================================================================
		//		グリッドの描画
		//================================================================================================
		Grid::Draw(camera);

		//================================================================================================
		//		振り子の描画
		//================================================================================================
		
		////スクリーン座標まで変換させる
		Vector3 anchorNdcPos = Matrix4x4::Transform(spring.anchor, camera->GetViewProjection());
		Vector3 anchorScreenPos = Matrix4x4::Transform(anchorNdcPos, camera->GetViewPort());
		Vector3 ballNdcPos = Matrix4x4::Transform(ball.position, camera->GetViewProjection());
		Vector3 ballScreenPos = Matrix4x4::Transform(ballNdcPos, camera->GetViewPort());
		////ひもの描画
		Novice::DrawLine(static_cast< int >(anchorScreenPos.x), static_cast< int >(anchorScreenPos.y),
						 static_cast< int >(ballScreenPos.x), static_cast< int >(ballScreenPos.y),
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
