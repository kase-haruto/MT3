#include <Novice.h>
#include<math.h>

const char kWindowTitle[] = "LE2A_09_カセ_ハルト";

struct Matrix4x4 {
	float m[4][4];
};

//	行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}
	return result;
}

//	行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}
	return result;
}

//	行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			float sum = 0.0f;
			for (int k = 0; k < 4; ++k) {
				sum += m1.m[i][k] * m2.m[k][j];
			}
			result.m[i][j] = sum;
		}
	}
	return result;
}


// 行列の逆行列を計算する関数
Matrix4x4 Inverse(const Matrix4x4& m) {
	// 拡大行列の作成
	float augmentedMatrix[4][8];
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			// 単位行列を右側に配置
			augmentedMatrix[i][j] = m.m[i][j];
			augmentedMatrix[i][j + 4] = (i == j) ? 1.0f : 0.0f;
		}
	}

	/*===============================================
		ガウス・ジョルダン法を使用した逆行列
	================================================*/

	for (int i = 0; i < 4; ++i) {

		float pivot = augmentedMatrix[i][i];

		for (int j = 0; j < 8; ++j) {
			augmentedMatrix[i][j] /= pivot;
		}

		// 他の行の要素を0にする
		for (int k = 0; k < 4; ++k) {
			if (k != i) {
				float factor = augmentedMatrix[k][i];
				for (int j = 0; j < 8; ++j) {
					augmentedMatrix[k][j] -= factor * augmentedMatrix[i][j];
				}
			}
		}
	}

	// 逆行列の取り出し
	Matrix4x4 invMatrix;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {

			invMatrix.m[i][j] = augmentedMatrix[i][j + 4];

		}
	}

	return invMatrix;
}


//	転置行列
Matrix4x4 Transpose(const Matrix4x4& m) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = m.m[j][i];
		}
	}
	return result;
}


static const int kRowHeight = 20;
static const int kColumnWidth = 60;

void MatrixScreenPrint(int x, int y, const Matrix4x4& mrix, const char* process) {
	Novice::ScreenPrintf(x, y, "%s", process);
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(
				x + column * kColumnWidth, (y + row * kRowHeight) + 20, "%6.2f", mrix.m[row][column]);
		}
	}
}

// 単位行列の作成
Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 identity;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (i == j) {
				identity.m[i][j] = 1.0f;
			} else {
				identity.m[i][j] = 0.0f;
			}
		}
	}
	return identity;
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Matrix4x4 m1 = { 3.2f,0.7f,9.6f,4.4f,
					5.5f,1.3f,7.8f,2.1f,
					6.9f,8.0f,2.6f,1.0f,
					0.5f,7.2f,5.1f,3.3f };

	Matrix4x4 m2 = { 4.1f,6.5f,3.3f,2.2f,
					8.8f,0.6f,9.9f,7.7f,
					1.1f,5.5f,6.6f,0.0f,
					3.3f,9.9f,8.8f,2.2f };

	Matrix4x4 resultAdd = Add(m1, m2);
	Matrix4x4 resultMultiply = Multiply(m1, m2);
	Matrix4x4 resultSubtract = Subtract(m1, m2);
	Matrix4x4 resulterseM1 = Inverse(m1);
	Matrix4x4 resulterseM2 = Inverse(m2);
	Matrix4x4 resultTranspose1 = Transpose(m1);
	Matrix4x4 resultTranspose2 = Transpose(m2);
	Matrix4x4 identity = MakeIdentity4x4();


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		MatrixScreenPrint(0, 0, resultAdd, "Add");
		MatrixScreenPrint(0, kRowHeight * 5, resultSubtract, "Subtract");
		MatrixScreenPrint(0, kRowHeight * 5 * 2, resultMultiply, "Multiply");
		MatrixScreenPrint(0, kRowHeight * 5 * 3, resulterseM1, "resulterseM1");
		MatrixScreenPrint(0, kRowHeight * 5 * 4, resulterseM2, "resulterseM2");
		MatrixScreenPrint(kColumnWidth * 5, 0, resultTranspose1, "transposeM1");
		MatrixScreenPrint(kColumnWidth * 5, kRowHeight * 5, resultTranspose2, "transposeM2");
		MatrixScreenPrint(kColumnWidth * 5, kRowHeight * 5 * 2, identity, "identity");

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
