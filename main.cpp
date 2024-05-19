#include<Novice.h>
#include<Matrix4x4.h>
#include<imgui.h>
#include"Sphere.h"
#include"Grid.h"
#include"MyFunc.h"
#include<memory>
#include"Camera.h"

const char kWindowTitle[] = "LE2A_09_カセ_ハルト_";

struct Plane{
	Vector3 normal;//法線
	float distance;//距離
};

void DrawPlane(const Plane& plane, const Camera* camera, uint32_t color){
	Vector3 center = plane.distance * plane.normal;//中心点を決める
	Vector3 perpendiculars[4];
	perpendiculars[0] = Vector3::Normalize(Perpendicular(plane.normal));//法線と垂直なベクトルを一つ求める
	perpendiculars[1] = -perpendiculars[0];//求めたベクトルの逆ベクトルを求める
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);//法線とクロス積を求める
	perpendiculars[3] = -perpendiculars[2];//求めたベクトルの逆ベクトルを求める
	//４頂点を求める
	Vector3 points[4];
	for (int32_t index = 0; index < 4; index++){
		Vector3 extend = perpendiculars[index] * 2.0f;
		Vector3 point = center + extend;
		points[index] = Matrix4x4::Transform(Matrix4x4::Transform(point, camera->GetViewProjection()), camera->GetViewPort());
	}

	//板の描画
	Novice::DrawLine(static_cast< int >(points[0].x), static_cast< int >(points[0].y),
					 static_cast< int >(points[2].x), static_cast< int >(points[2].y), color);
	Novice::DrawLine(static_cast< int >(points[1].x), static_cast< int >(points[1].y),
					 static_cast< int >(points[2].x), static_cast< int >(points[2].y), color);
	Novice::DrawLine(static_cast< int >(points[1].x), static_cast< int >(points[1].y),
					 static_cast< int >(points[3].x), static_cast< int >(points[3].y), color);
	Novice::DrawLine(static_cast< int >(points[3].x), static_cast< int >(points[3].y),
					 static_cast< int >(points[0].x), static_cast< int >(points[0].y), color);
}

bool IsCollision(Sphere* s,const Plane& plane){
	Vector3 spherePos = s->GetCenter();
	float sphereRadius = s->GetRadius();
	Vector3 pos = plane.distance * plane.normal;
	float distance;

	float d = Dot(pos, plane.normal);

	distance = std::abs(Dot(plane.normal, spherePos) - d);
	
	if (distance<=sphereRadius){
		return true;
	}
	return false;

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

	Vector3 s1Center = {0.0f,0.0f,0.0f};
	Vector3 rotate = {0.0f,0.0f,0.0f};
	float radius = 0.5f;
	uint32_t color = WHITE;

	std::unique_ptr<Sphere> sphere = std::make_unique<Sphere>();
	sphere->Init(s1Center, rotate, radius, color);

	Plane plane;
	plane.normal={0.0f,1.0f,0.0f};
	plane.distance = 1.0f;

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

		ImGui::Begin("plane");
		ImGui::DragFloat3("normal", &plane.normal.x, 0.01f);
		plane.normal = Vector3::Normalize(plane.normal);
		ImGui::DragFloat("distance", &plane.distance, 0.01f);
		ImGui::End();

		sphere->UpdateImGui("sphere");

		//================================================================================================
		//		カメラの行列の計算
		//================================================================================================
		camera->Update();


		//================================================================================================
		//		球の更新
		//================================================================================================		
		sphere->SetColor(WHITE);
		if (IsCollision(sphere.get(), plane)){
			sphere->SetColor(RED);
		}


		//================================================================================================
		//		グリッドの描画
		//================================================================================================
		Grid::Draw(camera);


		//================================================================================================
		//		球の描画
		//================================================================================================
		sphere->Draw(camera);

		//================================================================================================
		//		平面の描画
		//================================================================================================
		DrawPlane(plane, camera, WHITE);

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
