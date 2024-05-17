#include "Grid.h"
#include<stdint.h>
#include<Novice.h>

void Grid::DrawGridLine(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, const Vector3& startPos, const Vector3& endPos) {
	// ワールド座標からスクリーン座標へ変換
	Vector3 ndcStartPos = Matrix4x4::Transform(startPos, viewProjectionMatrix);
	Vector3 ndcEndPos = Matrix4x4::Transform(endPos, viewProjectionMatrix);
	Vector3 screenStartPos = Matrix4x4::Transform(ndcStartPos, viewportMatrix);
	Vector3 screenEndPos = Matrix4x4::Transform(ndcEndPos, viewportMatrix);

	// グリッドの描画
	Novice::DrawLine(
		int(screenStartPos.x), int(screenStartPos.y),
		int(screenEndPos.x), int(screenEndPos.y),
		0xaaaaaaff
	);
}

void Grid::Draw(const Camera* cam) {
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
		DrawGridLine(cam->GetViewProjection(), cam->GetViewPort(), worldStartPos, worldEndPos);
	}

	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		// グリッドの幅を均等に分割した位置を計算
		float zPos = -kGridHalfWidth + zIndex * kGridEvery;

		// 始点と終点のワールド座標を設定
		Vector3 worldStartPos(-kGridHalfWidth, 0.0f, zPos);
		Vector3 worldEndPos(kGridHalfWidth, 0.0f, zPos);

		// ラインを描画
		DrawGridLine(cam->GetViewProjection(), cam->GetViewPort(), worldStartPos, worldEndPos);
	}
}