#include"Matrix4x4.h"
#include<cmath>
#include<Novice.h>

float cot(float angle) {
	return 1 / std::tan(angle);
}

Matrix4x4 Matrix4x4::Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = 0.0f;
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	return result;
}

//正射影行列
Matrix4x4 Matrix4x4::MakeOrthographicMatrix(float l, float t, float r, float b, float nearClip, float farClip) {
	Matrix4x4 result;
	result = {
		2 / (r - l), 0, 0, 0,
		0, 2 / (t - b), 0, 0,
		0, 0, 1 / (farClip - nearClip), 0,
		(l + r) / (l - r), (t + b) / (b - t), nearClip / (nearClip - farClip), 1
	};
	return result;
}

//透視投影行列
Matrix4x4 Matrix4x4::MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 result;
	result = {
		1 / aspectRatio * cot(fovY / 2),0,0,0,
		0,cot(fovY / 2),0,0,
		0,0,farClip / (farClip - nearClip),1,
		0,0,-nearClip * farClip / (farClip - nearClip),0
	};
	return result;
}

//ビューポート変換行列
Matrix4x4 Matrix4x4::MakeViewportMatrix(float l, float t, float w, float h, float minDepth, float maxDepth) {
	Matrix4x4 result;
	result = {
		w / 2,0,0,0,
		0,-h / 2,0,0,
		0,0,maxDepth - minDepth,0,
		l + (w / 2),t + (h / 2),minDepth,1
	};
	return result;
}


void Matrix4x4::MatrixScreenPrint(int x, int y, const Matrix4x4& matrix, const char* str) {
	const int kRowHeight = 20;
	const int kColumnWidth = 60;

	Novice::ScreenPrintf(x, y, "%s", str);
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; column++) {
			Novice::ScreenPrintf(
				x + column * kColumnWidth, (y + row * kRowHeight) + kRowHeight, "%6.02f", matrix.m[row][column]
			);
		}
	}
}