#include<Novice.h>
#include<Matrix4x4.h>
#include<imgui.h>
#include"Sphere.h"
#include"Grid.h"
#include"MyFunc.h"
#include<memory>

const char kWindowTitle[] = "LE2A_09_カセ_ハルト_";


struct Camera{
	Vector3 translate;
	Vector3 rotate;
	Vector3 direction;
	Matrix4x4 cameraMatrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 worldViewProjectionMatrix;
	Matrix4x4 viewportMatrix;
};


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Camera cam;
	cam.translate = {0.0f,1.9f,-6.49f};
	cam.rotate = {0.26f,0.0f,0.0f};
	cam.projectionMatrix = Matrix4x4::MakePerspectiveFovMatrix(0.45f, 1280.0f / 720.0f, 0.1f, 100.0f);
	cam.viewportMatrix = Matrix4x4::MakeViewportMatrix(0, 0, 1280.0f, 720.0f, 0.0f, 1.0f);

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
		ImGui::Begin("camera");
		ImGui::DragFloat3("transtation", &cam.translate.x, 0.01f);
		ImGui::DragFloat3("rotation", &cam.rotate.x, 0.01f);
		ImGui::End();

		cam.cameraMatrix = Matrix4x4::MakeAffineMatrix({1.0f,1.0f,1.0f}, cam.rotate, cam.translate);
		cam.viewMatrix = Matrix4x4::Inverse(cam.cameraMatrix);
		Matrix4x4 viewProjectionMatrix = Matrix4x4::Multiply(cam.viewMatrix, cam.projectionMatrix);


		//================================================================================================
		//		球の更新
		//================================================================================================
		sphere1->UpdateImGui("sphere1");
		sphere2->UpdateImGui("sphere2");


		//================================================================================================
		//		グリッドの描画
		//================================================================================================
		Grid::Draw(viewProjectionMatrix, cam.viewportMatrix);


		//================================================================================================
		//		球の描画
		//================================================================================================
		sphere1->Draw(viewProjectionMatrix, cam.viewportMatrix);
		sphere2->Draw(viewProjectionMatrix, cam.viewportMatrix);

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
