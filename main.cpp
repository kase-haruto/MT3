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
#include<algorithm>


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

void AddPendulums(std::vector<Pendulum>& pendulums, std::vector<std::unique_ptr<Sphere>>& tips){
	// 新しい振り子を作成して初期化
	Pendulum newPendulum;
	float radius = tips[0]->GetRadius();
	newPendulum.anchor = {-0.7f + static_cast< int >(pendulums.size()) * (radius * 2.01f), 1.0f, 0.0f}; // アンカーの位置を調整
	newPendulum.length = 0.8f;
	newPendulum.angle = 0.0f;
	newPendulum.angularVelocity = 0.0f;
	newPendulum.angularAcceleration = 0.0f;
	newPendulum.mass = 2.0f;

	pendulums.push_back(newPendulum);

	// 新しい振り子の先端の球体を作成して初期化
	auto tip = std::make_unique<Sphere>();
	tip->Init(newPendulum.TipPosition(), {0.0f, 0.0f, 0.0f}, 0.1f, 0xFFFFFFff); // WHITE
	tips.push_back(std::move(tip));
}

void Stop(std::vector<Pendulum>& pendulums, bool& isMove){
	for (size_t i = 0; i < pendulums.size(); i++){
		pendulums[i].angularVelocity = 0.0f;
		pendulums[i].angle = 0.0f;
		pendulums[i].length = 0.8f;
	}
	isMove = false;
}

void Reset(std::vector<Pendulum>& pendulums, bool& isMove){
	for (size_t i = 0; i < pendulums.size(); i++){
		pendulums[i].angularVelocity = 0.0f;
		pendulums[i].angle = 0.0f;
		pendulums[i].length = 0.8f;
	}
	isMove = false;
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
	Pendulum pen;
	pen.anchor = {-0.7f,1.0f,0.0f};
	pen.length = 0.8f;
	pen.angle = 0.0f;
	pen.angularVelocity = 0.0f;
	pen.angularAcceleration = 0.0f;
	pen.mass = 2.0f;
	pendulums.push_back(pen);
	//振り子の先端の球体
	auto tip = std::make_unique<Sphere>();
	tip->Init(pen.TipPosition(), {0.0f, 0.0f, 0.0f}, 0.1f, WHITE);
	tips.push_back(std::move(tip));



	bool isMove = false;
	//bool isHit = false;

	float airResistanceCoefficient = 0.1f;
	float subSteps = 5;
	float subDeltaTime = deltaTime / subSteps;
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

	#ifdef _DEBUG
		ImGui::Begin("pendulums");
		if (ImGui::Button("start")){ isMove = true; }
		ImGui::SameLine();
		if (ImGui::Button("stop")){ isMove = false; }
		ImGui::SameLine();
		if (ImGui::Button("reset")){ Reset(pendulums, isMove); }
		ImGui::SameLine();
		if (ImGui::Button("addPendulum")){ AddPendulums(pendulums, tips); }

		for (size_t i = 0; i < pendulums.size(); ++i){
			std::string label = "Pendulum " + std::to_string(i);
			if (ImGui::TreeNode(label.c_str())){
				ImGui::DragFloat("Angle", &pendulums[i].angle, 0.01f);
				ImGui::DragFloat("Length", &pendulums[i].length, 0.01f);
				ImGui::SliderFloat("Mass", &pendulums[i].mass, 0.0f, 10.0f);
				ImGui::Text("velocity:%.1f", pendulums[i].angularVelocity);
				ImGui::TreePop();
			}
		}
		ImGui::End();
	#endif // _DEBUG



		//================================================================================================
		//		カメラの行列の計算
		//================================================================================================
		camera->Update();

		//================================================================================================
		// 振り子の計算
		//================================================================================================
		 // メインループ内での計算
		for (int step = 0; step < subSteps; ++step){
			// 振り子の計算
			for (size_t i = 0; i < pendulums.size(); ++i){
				tips[i]->SetPreCenter(pendulums[i].TipPosition());
				if (isMove){
					// 重力加速度と空気抵抗の計算
					float gravity_acceleration = -(gravity / pendulums[i].length) * std::sin(pendulums[i].angle);
					float air_resistance = -airResistanceCoefficient * pendulums[i].angularVelocity;

					// 角加速度の更新
					pendulums[i].angularAcceleration = gravity_acceleration + air_resistance;
					// 角速度の更新
					pendulums[i].angularVelocity += pendulums[i].angularAcceleration * subDeltaTime;
					// 角度の更新
					pendulums[i].angle += pendulums[i].angularVelocity * subDeltaTime;
				}
				tips[i]->SetCenter(pendulums[i].TipPosition());
			}

			//================================================================================================
			//      衝突処理
			//================================================================================================
			for (size_t i = 0; i < pendulums.size(); i++){
				tips[i]->SetColor(0xFFFFFFff); // WHITE
			}

			for (size_t i = 0; i < pendulums.size(); i++){
				for (size_t j = i + 1; j < pendulums.size(); j++){
					if (IsCollision(tips[i].get(), tips[j].get())){
						//わかりやすいように色を変える
						tips[i]->SetColor(0xFF0000ff); // RED
						tips[j]->SetColor(0xFF0000ff); // RED
						pendulums[i].angle = pendulums[j].angle;
						// 衝突後の角速度を計算（運動量保存の法則）
						float mass1 = pendulums[i].mass;
						float mass2 = pendulums[j].mass;
						float vel1 = pendulums[i].angularVelocity;
						float vel2 = pendulums[j].angularVelocity;

						//各速度を設定
						pendulums[i].angularVelocity = (vel1 * (mass1 - mass2) + 2 * mass2 * vel2) / (mass1 + mass2);
						pendulums[j].angularVelocity = (vel2 * (mass2 - mass1) + 2 * mass1 * vel1) / (mass1 + mass2);

					}
				}
			}
		}

		//================================================================================================
		//		グリッドの描画
		//================================================================================================
		Grid::Draw(camera);

		//================================================================================================
		//		振り子の描画
		//================================================================================================

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
