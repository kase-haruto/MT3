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
	newPendulum.anchor = {-0.6f + ( int ) pendulums.size() * 0.21f, 1.0f, 0.0f};
	newPendulum.length = 0.8f;
	newPendulum.angle = 0.0f;
	newPendulum.angularVelocity = 0.0f;
	newPendulum.angularAcceleration = 0.0f;
	newPendulum.mass = 1.0f;

	pendulums.push_back(newPendulum);

	// 新しい振り子の先端の球体を作成して初期化
	auto tip = std::make_unique<Sphere>();
	tip->Init(newPendulum.TipPosition(), {0.0f, 0.0f, 0.0f}, 0.1f, WHITE);
	tips.push_back(std::move(tip));
}

void Stop(std::vector<Pendulum>& pendulums,bool& isMove){
	for (size_t i = 0; i < pendulums.size(); i++){
		pendulums[i].angularVelocity = 0.0f;
		pendulums[i].angle = 0.0f;
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
	pen.anchor = {-0.6f,1.0f,0.0f};
	pen.length = 0.8f;
	pen.angle = 0.0f;
	pen.angularVelocity = 0.0f;
	pen.angularAcceleration = 0.0f;
	pen.mass = 1.0f;
	pendulums.push_back(pen);
	//振り子の先端の球体
	auto tip = std::make_unique<Sphere>();
	tip->Init(pen.TipPosition(), {0.0f, 0.0f, 0.0f}, 0.1f, WHITE);
	tips.push_back(std::move(tip));



	bool isMove = false;
	//bool isHit = false;

	

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
		ImGui::Begin("setting");
		if (ImGui::Button("start")){ isMove = true; }
		if ((ImGui::Button("stop"))){ Stop(pendulums, isMove); }
		for (size_t i = 0; i < pendulums.size(); ++i){
			std::string label = "Pendulum " + std::to_string(i);
			if (ImGui::TreeNode(label.c_str())){
				ImGui::DragFloat("Angle", &pendulums[i].angle, 0.01f);
				ImGui::DragFloat("Length", &pendulums[i].length, 0.01f);
				ImGui::DragFloat("Mass", &pendulums[i].mass, 0.01f);
				ImGui::TreePop();
			}
		}
		if (ImGui::Button("addPendulum")){ AddPendulums(pendulums, tips); }
		ImGui::End();
	#endif // _DEBUG

		

		//================================================================================================
		//		カメラの行列の計算
		//================================================================================================
		camera->Update();

		//================================================================================================
		//		振り子の計算
		//================================================================================================
		for (size_t i = 0; i < pendulums.size(); ++i){
			if (isMove){
				pendulums[i].angularAcceleration = -(gravity / pendulums[i].length) * std::sin(pendulums[i].angle);
				pendulums[i].angularVelocity += pendulums[i].angularAcceleration * deltaTime;
				pendulums[i].angle += pendulums[i].angularVelocity * deltaTime;
			}
			tips[i]->SetCenter(pendulums[i].TipPosition());
		}

		//================================================================================================
		//		衝突処理
		//================================================================================================
		for (size_t currentIndex = 0; currentIndex < pendulums.size(); currentIndex++){
			tips[currentIndex]->SetColor(WHITE);

			// 次の要素のインデックスを計算
			size_t next_index = (currentIndex + 1) % pendulums.size();

			if (tips.size() >= 2){
				// 現在の要素と次の要素の間の衝突判定
				if (IsCollision(tips[currentIndex].get(), tips[next_index].get())){
					tips[currentIndex]->SetColor(RED);

					// 衝突後の角速度を計算
					float tempAngularVelocity = pendulums[currentIndex].angularVelocity;
					pendulums[currentIndex].angularVelocity = pendulums[next_index].angularVelocity;
					pendulums[next_index].angularVelocity = tempAngularVelocity;


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
