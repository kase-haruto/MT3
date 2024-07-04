#include"Matrix4x4.h"
#include<cmath>
#include<assert.h>
#include"Vector4.h"
#include<Novice.h>

float cot(float angle) {
	return 1 / std::tan(angle);
}

Matrix4x4 Matrix4x4::MakeIdentity(){
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i){
		for (int j = 0; j < 4; ++j){
			if (i == j){
				result.m[i][j] = 1.0f; // 対角成分は1
			} else{
				result.m[i][j] = 0.0f; // 対角以外の成分は0
			}
		}
	}
	return result;
}

//平行移動行列
Matrix4x4 Matrix4x4::MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		translate.x,translate.y,translate.z,1
	};
	return result;
}

//拡大縮小行列
Matrix4x4 Matrix4x4::MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result = {
		scale.x,0,0,0,
		0,scale.y,0,0,
		0,0,scale.z,0,
		0,0,0,1
	};
	return result;
}

//回転行列
Matrix4x4 Matrix4x4::MakeRotateXMatrix(float theta) {
	Matrix4x4 result = {
		1,0,0,0,
		0,std::cos(theta),std::sin(theta),0,
		0,-std::sin(theta),std::cos(theta),0,
		0,0,0,1
	};

	return result;
}

Matrix4x4 Matrix4x4::MakeRotateYMatrix(float theta) {
	Matrix4x4 result = {
		std::cos(theta),0,-std::sin(theta),0,
		0,1,0,0,
		std::sin(theta),0,std::cos(theta),0,
		0,0,0,1
	};
	return result;
}

Matrix4x4 Matrix4x4::MakeRotateZMatrix(float theta) {
	Matrix4x4 result = {
		std::cos(theta),std::sin(theta),0,0,
		-std::sin(theta),std::cos(theta),0,0,
		0,0,1,0,
		0,0,0,1
	};
	return result;
}

Matrix4x4 Matrix4x4::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 affineMatrix;
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateMatrix = Matrix4x4::Multiply(Matrix4x4::Multiply(rotateXMatrix, rotateYMatrix), rotateZMatrix);

	affineMatrix = Matrix4x4::Multiply(Matrix4x4::Multiply(scaleMatrix, rotateMatrix), translateMatrix);

	return affineMatrix;
}

Matrix4x4 Matrix4x4::Inverse(const Matrix4x4& m) {
	Matrix4x4 invMatrix;

	float det =
		m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) +
		m.m[0][1] * (m.m[1][2] * m.m[2][0] - m.m[1][0] * m.m[2][2]) +
		m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]);

	if (det == 0) {
		return invMatrix; // ゼロ除算のエラー処理
	}

	float invDet = 1.0f / det;

	invMatrix.m[0][0] = (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) * invDet;
	invMatrix.m[0][1] = (m.m[0][2] * m.m[2][1] - m.m[0][1] * m.m[2][2]) * invDet;
	invMatrix.m[0][2] = (m.m[0][1] * m.m[1][2] - m.m[0][2] * m.m[1][1]) * invDet;
	invMatrix.m[0][3] = 0.0f;

	invMatrix.m[1][0] = (m.m[1][2] * m.m[2][0] - m.m[1][0] * m.m[2][2]) * invDet;
	invMatrix.m[1][1] = (m.m[0][0] * m.m[2][2] - m.m[0][2] * m.m[2][0]) * invDet;
	invMatrix.m[1][2] = (m.m[0][2] * m.m[1][0] - m.m[0][0] * m.m[1][2]) * invDet;
	invMatrix.m[1][3] = 0.0f;

	invMatrix.m[2][0] = (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]) * invDet;
	invMatrix.m[2][1] = (m.m[0][1] * m.m[2][0] - m.m[0][0] * m.m[2][1]) * invDet;
	invMatrix.m[2][2] = (m.m[0][0] * m.m[1][1] - m.m[0][1] * m.m[1][0]) * invDet;
	invMatrix.m[2][3] = 0.0f;

	invMatrix.m[3][0] = -(m.m[3][0] * invMatrix.m[0][0] + m.m[3][1] * invMatrix.m[1][0] + m.m[3][2] * invMatrix.m[2][0]);
	invMatrix.m[3][1] = -(m.m[3][0] * invMatrix.m[0][1] + m.m[3][1] * invMatrix.m[1][1] + m.m[3][2] * invMatrix.m[2][1]);
	invMatrix.m[3][2] = -(m.m[3][0] * invMatrix.m[0][2] + m.m[3][1] * invMatrix.m[1][2] + m.m[3][2] * invMatrix.m[2][2]);
	invMatrix.m[3][3] = 1.0f;

	return invMatrix;
}

//座標系変換
Vector3 Matrix4x4::Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result = { 0, 0, 0 };

	// 変換行列を適用
	Vector4 transformedVector(
		vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0],
		vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1],
		vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2],
		vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[3][3]
	);

	// 同次座標系から3次元座標系に戻す
	float w = transformedVector.w;
	if (w != 0.0f) {
		result.x = transformedVector.x / w;
		result.y = transformedVector.y / w;
		result.z = transformedVector.z / w;
	} else {
		// wが0の場合、適切な処理を行う（例：エラーハンドリング）
		// この例では、結果を0ベクトルとして返す
		result = { 0, 0, 0 };
	}

	return result;
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
		1 / aspectRatio * 1 / std::tan(fovY / 2),0,0,0,
		0,1 / std::tan(fovY / 2),0,0,
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

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& another) const{
	return Matrix4x4::Multiply(*this, another);
}