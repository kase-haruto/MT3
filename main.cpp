#include "Sphere.h"
#include "Grid.h"
#include "Camera.h"
#include "MyFunc.h"
#include "Collision.h"
#include "MyStruct.h"

#include <memory>
#include <Matrix4x4.h>
#include <Novice.h>
#include<vector>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

const char kWindowTitle[] = "LE2A_09_カセ_ハルト_";


//運動エネルギーの計算
float KineticEnergy(Pendulum& pen){
	return 0.5f * pen.mass * pen.length * pen.length * pen.angularVelocity * pen.angularVelocity;
}

//運動量を計算
float Momentum(Pendulum& pen){
	return pen.mass * pen.length * pen.length * pen.angularVelocity;
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Camera* camera = Camera::GetInstance();
	camera->Initialize();

	///=========================================================================================
	///			振り子の初期化
	///=========================================================================================
	std::vector<Pendulum> pendulums;
	std::vector<std::unique_ptr<Sphere>>tips;
	//振り子の数 = 5個
	pendulums.resize(5);
	for (size_t i = 0; i < pendulums.size(); ++i){
		pendulums[i].anchor = {-0.6f + i * 0.3f,1.0f,0.0f};
		pendulums[i].length = 0.8f;
		pendulums[i].angle = 0.0f;
		pendulums[i].angularVelocity = 0.0f;
		pendulums[i].angularAcceleration = -(gravity / pendulums[i].length) * std::sin(pendulums[i].angle);
		pendulums[i].mass = 1.0f;

		//振り子の先端の球体
		auto tip = std::make_unique<Sphere>();
		tip->Init(pendulums[i].TipPosition(), {0.0f, 0.0f, 0.0f}, 0.1f, WHITE);
		tips.push_back(std::move(tip));
	}
	//const float initialAngle = -0.7f;
	//// 最初の振り子の角度と角加速度を初期化
	//pendulums[0].angle = initialAngle;
	//pendulums[0].angularAcceleration = -(gravity / pendulums[0].length) * std::sin(pendulums[0].angle);
	//tips[0]->SetCenter(pendulums[0].TipPosition());


	bool isMove = false;
	bool isHit = false;

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
		ImGui::Begin("setting");
		if (ImGui::Button("start")){ isMove = true; }
		for (size_t i = 0; i < pendulums.size(); i++){
			ImGui::DragFloat3("pendulums[0]",)

		}
		ImGui::End();

		//================================================================================================
		//		カメラの行列の計算
		//================================================================================================
		camera->Update();

		//================================================================================================
		//		振り子の計算
		//================================================================================================
		if (isMove){
			for (size_t i = 0; i < pendulums.size(); ++i){
				pendulums[i].angularAcceleration = -(gravity / pendulums[i].length) * std::sin(pendulums[i].angle);
				pendulums[i].angularVelocity += pendulums[i].angularAcceleration * deltaTime;
				pendulums[i].angle += pendulums[i].angularVelocity * deltaTime;
				tips[i]->SetCenter(pendulums[i].TipPosition());
			}
		}

		//================================================================================================
		//		衝突処理
		//================================================================================================
		//tipB->SetColor(WHITE);
		//isHit = false;
		//for (size_t i = 0; i < pendulums.size(); ++i){
		//	if (IsCollision(tipA.get(), tipB.get())){
		//		tipB->SetColor(RED);
		//		isHit = true;
		//	}
		//}

		if (isHit){
			for (size_t i = 0; i < pendulums.size(); ++i){
				//// エネルギー伝達の計算
				float totalKineticEnergy = KineticEnergy(pendulums[i]) + KineticEnergy(pendulums[i + 1]);
				pendulums[i].angularVelocity = 0.0f;
				pendulums[i + 1].angularVelocity = std::sqrt(2 * totalKineticEnergy / (pendulums[i + 1].mass * pendulums[i + 1].length * pendulums[i + 1].length));
			}
		}

		//================================================================================================
		//		グリッドの描画
		//================================================================================================
		Grid::Draw(camera);

		//================================================================================================
		//		振り子の描画
		//================================================================================================

		//スクリーン座標まで変換させる

		//============================================================================================
		////		振り子A
		for (size_t i = 0; i < pendulums.size(); i++){
			Vector3 anchorNdcPos = Matrix4x4::Transform(pendulums[i].anchor, camera->GetViewProjection());
			Vector3 tipNdcPos = Matrix4x4::Transform(tips[i]->GetCenter(), camera->GetViewProjection());
			Vector3 anchorScreenPos = Matrix4x4::Transform(anchorNdcPos, camera->GetViewPort());
			Vector3 tipScreenPos = Matrix4x4::Transform(tipNdcPos, camera->GetViewPort());
			
			//ひもの描画
			Novice::DrawLine(static_cast< int >(anchorScreenPos.x), static_cast< int >(anchorScreenPos.y),
							 				 static_cast< int >(tipScreenPos.x), static_cast< int >(tipScreenPos.y),
							 				 WHITE);

			tips[i]->Draw(camera);
		}

		//Vector3 anchorNdcPos = Matrix4x4::Transform(pendulumA.anchor, camera->GetViewProjection());
		//Vector3 tipNdcPos = Matrix4x4::Transform(tipA->GetCenter(), camera->GetViewProjection());
		//Vector3 anchorScreenPos = Matrix4x4::Transform(anchorNdcPos, camera->GetViewPort());
		//Vector3 tipScreenPos = Matrix4x4::Transform(tipNdcPos, camera->GetViewPort());

		////============================================================================================
		////		振り子B
		//Vector3 anchorNdcPosB = Matrix4x4::Transform(pendulumB.anchor, camera->GetViewProjection());
		//Vector3 tipNdcPosB = Matrix4x4::Transform(tipB->GetCenter(), camera->GetViewProjection());
		//Vector3 anchorScreenPosB = Matrix4x4::Transform(anchorNdcPosB, camera->GetViewPort());
		//Vector3 tipScreenPosB = Matrix4x4::Transform(tipNdcPosB, camera->GetViewPort());

		////ひもの描画
		//Novice::DrawLine(static_cast< int >(anchorScreenPos.x), static_cast< int >(anchorScreenPos.y),
		//				 static_cast< int >(tipScreenPos.x), static_cast< int >(tipScreenPos.y),
		//				 WHITE);

		//Novice::DrawLine(static_cast< int >(anchorScreenPosB.x), static_cast< int >(anchorScreenPosB.y),
		//				 static_cast< int >(tipScreenPosB.x), static_cast< int >(tipScreenPosB.y),
		//				 WHITE);

		//tipA->Draw(camera);
		//tipB->Draw(camera);

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
