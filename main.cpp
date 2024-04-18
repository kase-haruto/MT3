#include <Novice.h>
#include<Matrix4x4.h>
#include<stdint.h>
#include<assert.h>
#include"Vector4.h"
#include"imgui.h"

const char kWindowTitle[] = "LE2A_09_カセ_ハルト_";

const int kWindowHeight = 720;
const int kWindowWidth = 1240;

const int kRowHeight = 20;
const int kColumnWidth = 60;
/// <summary>
/// 2つのベクトルの外積を計算します
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns>外積の結果</returns>
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

/// <summary>
/// 2つのベクトルの内積を計算します。
/// </summary>
/// <param name="v1">ベクトル1</param>
/// <param name="v2">ベクトル2</param>
/// <returns>内積の結果</returns>
float DotProduct(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}


struct Camera {
	Vector3 pos;
	Vector3 direction;
	Matrix4x4 cameraMatrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 worldViewProjectionMatrix;
	Matrix4x4 viewportMatrix;

	Vector3 GetDirection();
};

Vector3 Camera::GetDirection() {
	return Vector3(viewMatrix.m[0][2], viewMatrix.m[1][2], viewMatrix.m[2][2]);
}

struct Triangle {
	Matrix4x4 worldMatrix;
	Vector3 rotate;
	Vector3 translate;
	Vector3 ndcVertices;
	Vector3 kLocalVertices[3];
	Vector3 screenVertices[3];
	unsigned int color;

	bool DetermineTriangleFacing(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& camera_dir);
};

/// <summary>
/// 三角形がカメラから見て表向きならtrue、逆ならfalseを返す
/// </summary>
/// <param name="v1">三角形の頂点1</param>
/// <param name="v2">三角形の頂点2</param>
/// <param name="camera_dir">カメラの方向ベクトル</param>
/// <returns>表向きであればtrue、そうでなければfalse</returns>
bool Triangle::DetermineTriangleFacing(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& camera_dir) {
	// ドット積がc.(c*b) <= 0であれば表向き、それ以外は逆向きと判断する
	return DotProduct(camera_dir, Cross(v2 - v1, v3 - v2)) <= 0;
}

/// <summary>
/// 座標系を変換します。
/// </summary>
/// <param name="vector"></param>
/// <param name="matrix"></param>
/// <returns>変換後のベクトルを返返します</returns>
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result = { 0, 0, 0 };

	// 同次座標系への変換
	// 変換行列を適用
	Vector4 homogeneousCoordinate(
		vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0],
		vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1],
		vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2],
		vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[3][3]
	);

	// 同次座標系から3次元座標系に戻す
	float w = homogeneousCoordinate.w;
	assert(w != 0.0f); // wが0でないことを確認
	result.x = homogeneousCoordinate.x / w;
	result.y = homogeneousCoordinate.y / w;
	result.z = homogeneousCoordinate.z / w;

	return result;
}
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Camera cam;
	cam.pos = { 0.0f,0.0f,0.0f };
	Triangle tri;
	tri.rotate = { 0.0f,0.0f,0.0f };
	tri.translate = { 0.0f,0.0f,200.0f };
	tri.kLocalVertices[0] = { 0.0f,25.0f,0.0f };
	tri.kLocalVertices[1] = { 25.0f,-25.0f,0.0f };
	tri.kLocalVertices[2] = { -25.0f,-25.0f,0.0f };
	tri.color = RED;


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		ImGui::Begin("Triangle");
		ImGui::DragFloat3("Translate", &tri.translate.x, 0.01f);
		ImGui::DragFloat3("Rotate", &tri.rotate.x, 0.01f);
		ImGui::End();

		ImGui::Begin("camera");
		ImGui::DragFloat3("Translate", &cam.pos.x, 0.01f);
		ImGui::End();

		//================================================================================================
		//		三角形の移動
		//================================================================================================

		if (keys[DIK_A]) {
			tri.translate.x -= 5.0f;
		} else if (keys[DIK_D]) {
			tri.translate.x += 5.0f;
		}

		if (keys[DIK_W]) {
			tri.translate.z += 5.0f;
		}if (keys[DIK_S]) {
			tri.translate.z -= 5.0f;
		}

		//================================================================================================
		//		三角形の計算
		//================================================================================================

		//三角形の回転
		tri.rotate.y += 0.05f;
		//三角形の行列
		tri.worldMatrix = Matrix4x4::MakeAffineMatrix({ 1.0f,1.0f,1.0f }, tri.rotate, tri.translate);


		//================================================================================================
		//		カメラの行列の計算
		//================================================================================================

		//カメラの行列の計算
		cam.cameraMatrix = Matrix4x4::MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, cam.pos);
		cam.viewMatrix = Matrix4x4::Inverse(cam.cameraMatrix);
		cam.direction = cam.GetDirection();
		cam.projectionMatrix = Matrix4x4::MakePerspectiveFovMatrix(0.45f, 1280.0f / 720.0f, 0.1f, 100.0f);
		cam.worldViewProjectionMatrix = Matrix4x4::Multiply(tri.worldMatrix, Matrix4x4::Multiply(cam.viewMatrix, cam.projectionMatrix));
		cam.viewportMatrix = Matrix4x4::MakeViewportMatrix(0, 0, 1280.0f, 720.0f, 0.0f, 1.0f);


		//================================================================================================
		//		座標系の更新
		//================================================================================================

		for (uint32_t i = 0; i < 3; ++i) {
			tri.ndcVertices = Transform(tri.kLocalVertices[i], cam.worldViewProjectionMatrix);
			tri.screenVertices[i] = Transform(tri.ndcVertices, cam.viewportMatrix);
		}


		//================================================================================================
		//		三角形の描画
		//================================================================================================

		Novice::ScreenPrintf(0, 0, "%.1f", tri.rotate.y);

		// 三角形が表向きの場合のみ描画
		if (tri.DetermineTriangleFacing(tri.screenVertices[0], tri.screenVertices[1], tri.screenVertices[2], cam.direction) == 0) {
			Novice::DrawTriangle(
				int(tri.screenVertices[0].x), int(tri.screenVertices[0].y),
				int(tri.screenVertices[1].x), int(tri.screenVertices[1].y),
				int(tri.screenVertices[2].x), int(tri.screenVertices[2].y),
				tri.color, kFillModeSolid
			);
		}

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
