#include<Novice.h>
#include<Matrix4x4.h>
#include<imgui.h>
#include"Sphere.h"
#include"Grid.h"

const char kWindowTitle[] = "LE2A_09_カセ_ハルト_";


struct Camera {
	Vector3 translate;
	Vector3 rotate;
	Vector3 direction;
	Matrix4x4 cameraMatrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 worldViewProjectionMatrix;
	Matrix4x4 viewportMatrix;
};

struct Line {
	Vector3 origin;//始点
	Vector3 diff;//終点への線分ベクトル
};

struct Ray {
	Vector3 origin;
	Vector3 diff;
};

struct Segment {
	Vector3 origin;
	Vector3 diff;
};


/// <summary>
/// ベクトルの内積を求めます
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns>ベクトルの内積</returns>
float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/// <summary>
/// 正射影ベクトルを求めます
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns>計算結果を返します</returns>
Vector3 Projection(const Vector3& v1, const Vector3& v2) {
	Vector3 b = Vector3::Normalize(v2);
	float d = Dot(v1, b);
	return b * d;
}

/// <summary>
/// 最近接点を求めます
/// </summary>
/// <param name="point"></param>
/// <param name="segment"></param>
/// <returns>最近接点の値を返します</returns>
Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
	return segment.origin + Projection((point - segment.origin), segment.diff);
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Camera cam;
	cam.translate = { 0.0f,1.9f,-6.49f };
	cam.rotate = { 0.26f,0.0f,0.0f };
	cam.projectionMatrix = Matrix4x4::MakePerspectiveFovMatrix(0.45f, 1280.0f / 720.0f, 0.1f, 100.0f);
	cam.viewportMatrix = Matrix4x4::MakeViewportMatrix(0, 0, 1280.0f, 720.0f, 0.0f, 1.0f);

	Segment segment{ {-2.0f,-1.0f,0.0f},{3.0f,2.0f,2.0f} };
	Vector3 point{ -1.5f,0.6f,0.6f };

	Vector3 project = Projection((point - segment.origin), segment.diff);
	Vector3 closestPoint = ClosestPoint(point, segment);

	Vector3  start;
	Vector3 end;

	Sphere* pointSphere = new Sphere(point, 0.01f, RED);
	Sphere* closestPointSphere = new Sphere(closestPoint, 0.01f, BLACK);

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);


		ImGui::Begin("segment");
		ImGui::DragFloat3("point", &point.x, 0.01f);
		ImGui::DragFloat3("origin", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("diff", &segment.diff.x, 0.01f);
		ImGui::InputFloat3("Project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::End();

		ImGui::Begin("camera");
		ImGui::DragFloat3("translate", &cam.translate.x, 0.01f);
		ImGui::DragFloat3("rotate", &cam.rotate.x, 0.01f);
		ImGui::End();

		//================================================================================================
		//		カメラの行列の計算
		//================================================================================================
		cam.cameraMatrix = Matrix4x4::MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cam.rotate, cam.translate);
		cam.viewMatrix = Matrix4x4::Inverse(cam.cameraMatrix);
		Matrix4x4 viewProjectionMatrix = Matrix4x4::Multiply(cam.viewMatrix, cam.projectionMatrix);

		//================================================================================================
		//		線分の計算
		//================================================================================================
		start = Matrix4x4::Transform(Matrix4x4::Transform(segment.origin, viewProjectionMatrix), cam.viewportMatrix);
		end = Matrix4x4::Transform(Matrix4x4::Transform((segment.origin + segment.diff), viewProjectionMatrix), cam.viewportMatrix);



		//================================================================================================
		//		グリッドの描画
		//================================================================================================
		Grid::Draw(viewProjectionMatrix, cam.viewportMatrix);

		//================================================================================================
		//		線分の描画
		//================================================================================================
		Novice::DrawLine(
			int(start.x), int(start.y),
			int(end.x), int(end.y), WHITE
		);

		//================================================================================================
		//		球体の描画
		//================================================================================================
		pointSphere->Draw(viewProjectionMatrix, cam.viewportMatrix);
		closestPointSphere->Draw(viewProjectionMatrix, cam.viewportMatrix);



		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	delete pointSphere, closestPointSphere;

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
