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

//void Grid::Draw(const Camera* cam, const float kGridHalfWidth) {
//	const uint32_t kSubdivision = (int)kGridHalfWidth*5;
//	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);
//
//	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
//		// グリッドの幅を均等に分割した位置を計算
//		float xPos = -kGridHalfWidth + xIndex * kGridEvery;
//
//		// 始点と終点のワールド座標を設定
//		Vector3 worldStartPos(xPos, 0.0f, kGridHalfWidth);
//		Vector3 worldEndPos(xPos, 0.0f, -kGridHalfWidth);
//
//		// ラインを描画
//		DrawGridLine(cam->GetViewProjection(), cam->GetViewPort(), worldStartPos, worldEndPos);
//	}
//
//	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
//		// グリッドの幅を均等に分割した位置を計算
//		float zPos = -kGridHalfWidth + zIndex * kGridEvery;
//
//		// 始点と終点のワールド座標を設定
//		Vector3 worldStartPos(-kGridHalfWidth, 0.0f, zPos);
//		Vector3 worldEndPos(kGridHalfWidth, 0.0f, zPos);
//
//		// ラインを描画
//		DrawGridLine(cam->GetViewProjection(), cam->GetViewPort(), worldStartPos, worldEndPos);
//	}
//}

void Grid::Draw(const Camera* cam, const float kGridHalfWidth){
    const uint32_t kSubdivision = ( int ) kGridHalfWidth * 5;
    const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);

    for (uint32_t xIndex = 0; xIndex < kSubdivision; ++xIndex){
        for (uint32_t zIndex = 0; zIndex < kSubdivision; ++zIndex){
            // 各セルの四隅のワールド座標を計算
            float xPos1 = -kGridHalfWidth + xIndex * kGridEvery;
            float xPos2 = xPos1 + kGridEvery;
            float zPos1 = -kGridHalfWidth + zIndex * kGridEvery;
            float zPos2 = zPos1 + kGridEvery;

            Vector3 worldPosA(xPos1, 0.0f, zPos1);
            Vector3 worldPosB(xPos2, 0.0f, zPos1);
            Vector3 worldPosC(xPos2, 0.0f, zPos2);
            Vector3 worldPosD(xPos1, 0.0f, zPos2);

            // NDC座標に変換
            Vector3 ndcPosA = Matrix4x4::Transform(worldPosA, cam->GetViewProjection());
            Vector3 ndcPosB = Matrix4x4::Transform(worldPosB, cam->GetViewProjection());
            Vector3 ndcPosC = Matrix4x4::Transform(worldPosC, cam->GetViewProjection());
            Vector3 ndcPosD = Matrix4x4::Transform(worldPosD, cam->GetViewProjection());

            // スクリーン座標に変換
            Vector3 screenPosA = Matrix4x4::Transform(ndcPosA, cam->GetViewPort());
            Vector3 screenPosB = Matrix4x4::Transform(ndcPosB, cam->GetViewPort());
            Vector3 screenPosC = Matrix4x4::Transform(ndcPosC, cam->GetViewPort());
            Vector3 screenPosD = Matrix4x4::Transform(ndcPosD, cam->GetViewPort());

            // 四角形を描画（2つの三角形を使って塗りつぶす）
            Novice::DrawTriangle(int(screenPosA.x), int(screenPosA.y),
                                 int(screenPosB.x), int(screenPosB.y),
                                 int(screenPosC.x), int(screenPosC.y), 0x5c5c5caa, kFillModeSolid);

            Novice::DrawTriangle(int(screenPosA.x), int(screenPosA.y),
                                 int(screenPosC.x), int(screenPosC.y),
                                 int(screenPosD.x), int(screenPosD.y), 0x5c5c5caa,kFillModeSolid);
        }
    }
}
