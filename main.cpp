#include "Sphere.h"
#include "Grid.h"
#include "Camera.h"
#include "MyFunc.h"
#include "Collision.h"
#include "MyStruct.h"
#include "Ball.h"

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

void AddBalls(std::vector<std::unique_ptr<Ball>>& balls){
	std::unique_ptr<Ball> ball = std::make_unique<Ball>();

	// ボールの半径
	float radius = 0.25f;
	// 隙間の量を定義
	float gap = 0.01f; // 隙間の量を調整

	int n = static_cast< int >(balls.size()) - 1; // 最初のボールを除外

	float x, z;

	if (n == 0){
		// 2つ目のボールを頂点に配置
		x = 0.0f;
		z = 0.0f;
	} else{
		// それ以降のボールをピラミッド形状に配置
		int row = 0;
		int balls_in_previous_rows = 0;

		// 行の計算
		while (balls_in_previous_rows + (row + 1) <= n){
			row++;
			balls_in_previous_rows += row;
		}

		int index_in_row = n - balls_in_previous_rows;

		x = (index_in_row - row / 2.0f) * (radius * 2.0f + gap) + (radius + gap / 2.0f) * (row % 2 == 0 ? 0.5f : 0.0f);
		z = row * (radius * 2.0f + gap);
	}

	ball->Initialize({x, 0.0f, z}, radius, 1.0f, BLACK);
	balls.push_back(std::move(ball));
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


	///=========================================================================================
	///			シーン２ビリヤード
	///=========================================================================================

	std::vector<std::unique_ptr<Ball>> balls;
	std::unique_ptr<Ball> ball = std::make_unique<Ball>();
	ball->Initialize({0.0f,0.0f,-3.0f}, 0.25f,1.0f, WHITE);
	Vector3 initVel = {0.0f,0.0f,0.07f};
	ball->SetVelocity(initVel);
	balls.push_back(std::move(ball));

	

	bool isMoveBalls = false;
	int scene = 1;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0){
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

	#ifdef _DEBUG

		ImGui::SliderInt("scene", &scene, 0, 1);

	#endif // _DEBUG

		//================================================================================================
		//		カメラの行列の計算
		//================================================================================================
		camera->Update();

		switch (scene){
			case 0:

			#pragma region

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

			#pragma endregion

				break;

			case 1:

				//================================================================================================
				//		imguiの更新
				//================================================================================================

			#ifdef _DEBUG

				ImGui::Begin("sphere");
				if (ImGui::Button("start")){ isMoveBalls = true; }
				ImGui::SameLine();
				if (ImGui::Button("stop")){ isMoveBalls = false; }
				ImGui::SameLine();
				if (ImGui::Button("addPendulum")){ AddBalls(balls); }
				ImGui::End();

			#endif // _DEBUG

				
					//========================================================================
					//		ボールの更新処理
					//========================================================================
					if (isMoveBalls){
						for (size_t i = 0; i < balls.size(); i++){
							balls[i]->Update();
						}
					}
					for (int step = 0; step < subSteps; ++step){
						//========================================================================
						//		球体同士の衝突判定
						//========================================================================
						if (balls.size() >= 2){
							for (size_t i = 0; i < balls.size(); i++){
								for (size_t j = i + 1; j < balls.size(); j++){
									if (IsCollision(balls[i]->GetSphere(), balls[j]->GetSphere())){

										// 球の情報を取得
										float mass1 = balls[i]->GetMass();
										float mass2 = balls[j]->GetMass();
										Vector3 vel1 = balls[i]->GetVelocity();
										Vector3 vel2 = balls[j]->GetVelocity();

										Vector3 pos1 = balls[i]->GetCenter();
										Vector3 pos2 = balls[j]->GetCenter();

										// 衝突の法線方向を計算
										Vector3 normal = Vector3::Normalize(pos2 - pos1);

										// 速度の内積を計算
										float v1n = Dot(vel1, normal);
										float v2n = Dot(vel2, normal);

										// 新しい法線方向の速度を計算（運動量保存の法則と完全弾性衝突を仮定）
										float v1nNew = (v1n * (mass1 - mass2) + 2.0f * mass2 * v2n) / (mass1 + mass2);
										float v2nNew = (v2n * (mass2 - mass1) + 2.0f * mass1 * v1n) / (mass1 + mass2);

										// 法線方向の速度を更新
										Vector3 v1nNewVec = normal * v1nNew;
										Vector3 v2nNewVec = normal * v2nNew;

										// 接線方向の速度を計算（変わらない）
										Vector3 v1t = vel1 - normal * v1n;
										Vector3 v2t = vel2 - normal * v2n;

										// 新しい速度を計算
										Vector3 newVel1 = v1t + v1nNewVec;
										Vector3 newVel2 = v2t + v2nNewVec;

										balls[i]->SetVelocity(newVel1);
										balls[j]->SetVelocity(newVel2);
									}
								}
							}
						}
					}


				//==========================================================================
				//		グリッドの描画
				//==========================================================================
				Grid::Draw(camera);

				//========================================================================
				//		球体の描画
				//========================================================================
				for (int i = 0; i < balls.size(); i++){
					balls[i]->Draw(camera);
				}


				break;
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
