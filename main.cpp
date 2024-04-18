#include<Matrix4x4.h>
#include<stdint.h>
#include<assert.h>
#include"Vector4.h"
#include<imgui.h>
#define _USE_MATH_DEFINES
#include<cmath>

const char kWindowTitle[] = "LE2A_09_カセ_ハルト_";

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
	Vector3 translate;
	Vector3 rotate;
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

struct Sphere {
	Vector3 center;
	Vector3 rotate;
	float radius;
	Matrix4x4 worldMatrix;
	uint32_t color;
};


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



/*==================================================================================================
		グリッド線を引く関数
===================================================================================================*/

/// <summary>
/// グリッドを描画します
/// </summary>
/// <param name="viewProjectionMatrix"></param>
/// <param name="viewportMatrix"></param>
/// <param name="startPos"></param>
/// <param name="endPos"></param>
void DrawGridLine(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, const Vector3& startPos, const Vector3& endPos) {
	// ワールド座標からスクリーン座標へ変換
	Vector3 ndcStartPos = Transform(startPos, viewProjectionMatrix);
	Vector3 ndcEndPos = Transform(endPos, viewProjectionMatrix);
	Vector3 screenStartPos = Transform(ndcStartPos, viewportMatrix);
	Vector3 screenEndPos = Transform(ndcEndPos, viewportMatrix);

	// グリッドの描画
	Novice::DrawLine(
		int(screenStartPos.x), int(screenStartPos.y),
		int(screenEndPos.x), int(screenEndPos.y),
		0xaaaaaaff
	);
}

/// <summary>
/// 縦横の線を描画します
/// </summary>
/// <param name="viewProjectionMatrix"></param>
/// <param name="viewportMatrix"></param>
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);

	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		// グリッドの幅を均等に分割した位置を計算
		float xPos = -kGridHalfWidth + xIndex * kGridEvery;

		// 始点と終点のワールド座標を設定
		Vector3 worldStartPos(xPos, 0.0f, kGridHalfWidth);
		Vector3 worldEndPos(xPos, 0.0f, -kGridHalfWidth);

		// ラインを描画
		DrawGridLine(viewProjectionMatrix, viewportMatrix, worldStartPos, worldEndPos);
	}

	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		// グリッドの幅を均等に分割した位置を計算
		float zPos = -kGridHalfWidth + zIndex * kGridEvery;

		// 始点と終点のワールド座標を設定
		Vector3 worldStartPos(-kGridHalfWidth, 0.0f, zPos);
		Vector3 worldEndPos(kGridHalfWidth, 0.0f, zPos);

		// ラインを描画
		DrawGridLine(viewProjectionMatrix, viewportMatrix, worldStartPos, worldEndPos);
	}
}



/*==================================================================================================
		球体を描画
===================================================================================================*/
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	// 分割数
	const uint32_t kSubdivision = 16; // 任意の適切な値を設定
	// 軽度分割1つ分の角度
	const float kLonEvery = 2 * float(M_PI) / kSubdivision;
	// 緯度分割1つ分の角度
	const float kLatEvery = float(M_PI) / kSubdivision;
	Vector3 a, b, c;

	Vector3 ndcPosA;
	Vector3 ndcPosB;
	Vector3 ndcPosC;

	Vector3 screenPosA;
	Vector3 screenPosB;
	Vector3 screenPosC;

	//緯度の方向に分割　-n/2 ~ n/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -float(M_PI) / 2.0f + kLatEvery * latIndex;
		//軽度の方向に分割 0~2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;

			// 点の座標を計算
			a.x = sphere.radius * (std::cos(lat) * std::cos(lon)) + sphere.center.x;
			a.y = sphere.radius * std::sin(lat) + sphere.center.y;
			a.z = sphere.radius * (std::cos(lat) * std::sin(lon)) + sphere.center.z;

			b.x = sphere.radius * (std::cos(lat + kLatEvery) * std::cos(lon)) + sphere.center.x;
			b.y = sphere.radius * std::sin(lat + kLatEvery) + sphere.center.y;
			b.z = sphere.radius * (std::cos(lat + kLatEvery) * std::sin(lon)) + sphere.center.z;

			c.x = sphere.radius * (std::cos(lat) * std::cos(lon + kLonEvery)) + sphere.center.x;
			c.y = sphere.radius * std::sin(lat) + sphere.center.y;
			c.z = sphere.radius * (std::cos(lat) * std::sin(lon + kLonEvery)) + sphere.center.z;

			//スクリーン座標まで変換させる
			ndcPosA = Transform(a, viewProjectionMatrix);
			ndcPosB = Transform(b, viewProjectionMatrix);
			ndcPosC = Transform(c, viewProjectionMatrix);

			screenPosA = Transform(ndcPosA, viewportMatrix);
			screenPosB = Transform(ndcPosB, viewportMatrix);
			screenPosC = Transform(ndcPosC, viewportMatrix);

			//ab,bcで線を引く
			Novice::DrawLine(int(screenPosA.x), int(screenPosA.y), int(screenPosB.x), int(screenPosB.y), sphere.color);
			Novice::DrawLine(int(screenPosA.x), int(screenPosA.y), int(screenPosC.x), int(screenPosC.y), sphere.color);
		}
	}
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

	Sphere sphere;
	sphere.center = { 0.0f,0.0f,0.0f };
	sphere.radius = 0.5f;
	sphere.color = BLACK;


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		ImGui::Begin("camera");
		ImGui::DragFloat3("translate", &cam.translate.x, 0.01f);
		ImGui::DragFloat3("rotate", &cam.rotate.x, 0.01f);
		ImGui::End();

		ImGui::Begin("sphere");
		ImGui::DragFloat3("center", &sphere.center.x, 0.01f);
		ImGui::DragFloat("radius", &sphere.radius, 0.01f);
		ImGui::End();


		Matrix4x4 viewProjectionMatrix = Matrix4x4::Multiply(cam.viewMatrix, cam.projectionMatrix);


		//================================================================================================
		//		グリッドの描画
		//================================================================================================
		DrawGrid(viewProjectionMatrix, cam.viewportMatrix);

		//================================================================================================
		//		球体
		//================================================================================================

		DrawSphere(sphere, viewProjectionMatrix, cam.viewportMatrix);

		//================================================================================================
		//		カメラの行列の計算
		//================================================================================================
		cam.cameraMatrix = Matrix4x4::MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cam.rotate, cam.translate);
		cam.viewMatrix = Matrix4x4::Inverse(cam.cameraMatrix);


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
