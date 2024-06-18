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
#include"Sphere.h"

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

	std::unique_ptr<OBB> obb = std::make_unique<OBB>();
	Vector3 obbCenter = {-1.0f,0.0f,0.0f};
	Vector3 rotate = {0.0f,0.0f,0.0f};
	Vector3 size = {0.5f,0.5f,0.5f};
	obb->Initialize(obbCenter, rotate, size);

	std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>();
	Vector3 sphereCenter = {0.0f,0.0f,0.0f};
	sphere->Init(sphereCenter, {0.0f,0.0f,0.0f}, 0.5f, WHITE);

	//obbと球
	//sphereとobbどっちにもmInverseをかける


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
		obb->UpdateUI("obb");
		sphere->UpdateImGui("sphere");
		//================================================================================================
		//		カメラの行列の計算
		//================================================================================================
		camera->Update();

		//================================================================================================
		//		線分の計算
		//================================================================================================
		//segmentEnd = segment.diff + segment.origin;

		
		//================================================================================================
		//		obb更新
		//================================================================================================
		obb->Update();

		//================================================================================================
		//		衝突判定
		//================================================================================================
		sphere->SetColor(WHITE);
		if (IsCollision(obb.get(), sphere.get())){
			sphere->SetColor(RED);
		}


		//================================================================================================
		//		グリッドの描画
		//================================================================================================
		Grid::Draw(camera);

		//================================================================================================
		//		線分の描画
		//================================================================================================

	/*	Vector3 ndcStartPos = Matrix4x4::Transform(segment.origin, camera->GetViewProjection());
		Vector3 ndcEndPos = Matrix4x4::Transform(segmentEnd, camera->GetViewProjection());
		Vector3 screenStartPos = Matrix4x4::Transform(ndcStartPos, camera->GetViewPort());
		Vector3 screenEndPos = Matrix4x4::Transform(ndcEndPos, camera->GetViewPort());
		Novice::DrawLine(static_cast< int >(screenStartPos.x), static_cast< int >(screenStartPos.y),
						 static_cast< int >(screenEndPos.x), static_cast< int >(screenEndPos.y),
						 segment.color);*/


		//================================================================================================
		//		obbの描画
		//================================================================================================
		obb->Draw(camera);

		
		//================================================================================================
		//		球体の描画
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
