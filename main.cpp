#include"Camera.h"
#include"MyFunc.h"
#include"MyStruct.h"
#include"Grid.h"
#include"Sphere.h"
#include"Transform.h"

#include<array>
#include<memory>
#include<Matrix4x4.h>
#include<Novice.h>

#ifdef _DEBUG
#include<imgui.h>
#endif // _DEBUG

const char kWindowTitle[] = "LE2A_09_カセ_ハルト_";


enum Parts{
	Shoulder,
	Elbow,
	Hand,
	partsCount
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Camera* camera = Camera::GetInstance();
	camera->Initialize();

	Transform transforms[3];
	transforms[Shoulder].translation = {0.2f, 1.0f, 0.0f};
	transforms[Shoulder].rotation = {0.0f, 0.0f, -6.8f};
	transforms[Shoulder].scale = {1.0f, 1.0f, 1.0f};

	transforms[Elbow].translation = {0.4f, 0.0f, 0.0f};
	transforms[Elbow].rotation = {0.0f, 0.0f, -1.4f};
	transforms[Elbow].scale = {1.0f, 1.0f, 1.0f};

	transforms[Hand].translation = {0.3f, 0.0f, 0.0f};
	transforms[Hand].rotation = {0.0f, 0.0f, 0.0f};
	transforms[Hand].scale = {1.0f, 1.0f, 1.0f};

	// 親子関係を結ぶ
	Matrix4x4 matWorlds[partsCount];
	matWorlds[Shoulder] = Matrix4x4::MakeAffineMatrix(transforms[Shoulder].scale, transforms[Shoulder].rotation, transforms[Shoulder].translation);
	matWorlds[Elbow] = Matrix4x4::Multiply(Matrix4x4::MakeAffineMatrix(transforms[Elbow].scale, transforms[Elbow].rotation, transforms[Elbow].translation), matWorlds[Shoulder]);
	matWorlds[Hand] = Matrix4x4::Multiply(Matrix4x4::MakeAffineMatrix(transforms[Hand].scale, transforms[Hand].rotation, transforms[Hand].translation), matWorlds[Elbow]);

	auto getWorldPos = [] (const Matrix4x4& mat)->Vector3{
		return Vector3(mat.m[3][0], mat.m[3][1], mat.m[3][2]);
		};

	std::array<std::unique_ptr<Sphere>, partsCount> spheres;
	for (int i = 0; i < partsCount; ++i){
		spheres[i] = std::make_unique<Sphere>();
	}

	spheres[Shoulder]->Init(getWorldPos(matWorlds[Shoulder]), {0.0f, 0.0f, 0.0f}, 0.08f, RED);
	spheres[Elbow]->Init(getWorldPos(matWorlds[Elbow]), {0.0f, 0.0f, 0.0f}, 0.08f, GREEN);
	spheres[Hand]->Init(getWorldPos(matWorlds[Hand]), {0.0f, 0.0f, 0.0f}, 0.08f, BLUE);


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

		ImGui::Text("Shoulder");
		ImGui::DragFloat3("ShoulderTranslation", &transforms[Shoulder].translation.x, 0.01f);
		ImGui::DragFloat3("ShoulderRotation", &transforms[Shoulder].rotation.x, 0.01f);

		ImGui::Text("Elbow");
		ImGui::DragFloat3("ElbowTranslation", &transforms[Elbow].translation.x, 0.01f);
		ImGui::DragFloat3("ElbowRotation", &transforms[Elbow].rotation.x, 0.01f);

		ImGui::Text("Hand");
		ImGui::DragFloat3("HandTranslation", &transforms[Hand].translation.x, 0.01f);
		ImGui::DragFloat3("HandRotation", &transforms[Hand].rotation.x, 0.01f);

		ImGui::End();



		//================================================================================================
		//		カメラの行列の計算
		//================================================================================================
		camera->Update();


		//================================================================================================
		//		パーツの行列の更新
		//================================================================================================
		// パーツの行列の更新
		matWorlds[Shoulder] = Matrix4x4::MakeAffineMatrix(transforms[Shoulder].scale, transforms[Shoulder].rotation, transforms[Shoulder].translation);
		matWorlds[Elbow] = Matrix4x4::Multiply(Matrix4x4::MakeAffineMatrix(transforms[Elbow].scale, transforms[Elbow].rotation, transforms[Elbow].translation), matWorlds[Shoulder]);
		matWorlds[Hand] = Matrix4x4::Multiply(Matrix4x4::MakeAffineMatrix(transforms[Hand].scale, transforms[Hand].rotation, transforms[Hand].translation), matWorlds[Elbow]);

		// 球体の位置の更新
		spheres[Shoulder]->SetCenter(getWorldPos(matWorlds[Shoulder]));
		spheres[Elbow]->SetCenter(getWorldPos(matWorlds[Elbow]));
		spheres[Hand]->SetCenter(getWorldPos(matWorlds[Hand]));

		//================================================================================================
		//		グリッドの描画
		//================================================================================================
		Grid::Draw(camera);


		//================================================================================================
		//		球体の描画
		//================================================================================================
		for (const auto& sphere : spheres){
			sphere->Draw(camera);
		}

		auto worldToScreen = [] (const Vector3& worldPos, const Camera* camera)->Vector3{
			Vector3 ndcPos = Matrix4x4::Transform(worldPos, camera->GetViewProjection());
			Vector3 screenPos = Matrix4x4::Transform(ndcPos, camera->GetViewPort());
			return screenPos;
			};

		Vector3 screenShoulder = worldToScreen(spheres[Shoulder]->GetCenter(),camera);
		Vector3 screenElbow = worldToScreen(spheres[Elbow]->GetCenter(),camera);
		Vector3 screenHand= worldToScreen(spheres[Hand]->GetCenter(),camera);

		Novice::DrawLine(static_cast< int >(screenShoulder.x), static_cast< int >(screenShoulder.y),
						 static_cast< int >(screenElbow.x), static_cast< int >(screenElbow.y), WHITE);

		Novice::DrawLine(static_cast< int >(screenElbow.x), static_cast< int >(screenElbow.y),
						 static_cast< int >(screenHand.x), static_cast< int >(screenHand.y), WHITE);

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
