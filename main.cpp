#include <Novice.h>
#include<math.h>

const char kWindowTitle[] = "LE2A_09_カセ_ハルト";

const int kColumWidth = 60;
const int kRowHeight = 60;

struct Vec3f {
	float x;
	float y;
	float z;
};

//加算
Vec3f Add(const Vec3f& v1, const Vec3f& v2) {
	return { v1.x + v2.x,v1.y + v2.y,v1.z + v2.z };
}

//減算
Vec3f Subtract(const Vec3f& v1, const Vec3f& v2) {
	return { v1.x - v2.x,v1.y - v2.y,v1.z - v2.z };
}

//乗算
Vec3f Multiply(const Vec3f& v1, const float k) {
	return { v1.x * k,v1.y * k,v1.z * k };
}

//内積
float Dot(const Vec3f& v1, const Vec3f& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

//長さ
float Length(const Vec3f& v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

//正規化
Vec3f Normalize(const Vec3f& v) {
	Vec3f unitVec3;
	if (Length(v) != 0) {
		unitVec3.x /= Length(v);
		unitVec3.y /= Length(v);
		unitVec3.z /= Length(v);
	}
	return unitVec3;
}


void PrintVector3(int x, int y, const Vec3f& v, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", v.x);
	Novice::ScreenPrintf(x + kColumWidth, y, "%.02f", v.y);
	Novice::ScreenPrintf(x + kColumWidth * 2, y, "%.02f", v.z);
	Novice::ScreenPrintf(x + kColumWidth * 3, y, "%s", label);
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vec3f v1 = { 1.0f,3.0f,-5.0f };
	Vec3f v2 = { 4.0f,-1.0f,2.0f };
	float k = 4.0f;

	Vec3f addResult = Add(v1, v2);
	Vec3f subtractResult = Subtract(v1, v2);
	Vec3f multiplyResult = Multiply(v1, k);
	float dotResult = Dot(v1, v2);
	float lengthResult = Length(v1);
	Vec3f normalizeResult = Normalize(v2);


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		PrintVector3(0, 0, addResult, ":add");
		PrintVector3(0, kRowHeight, subtractResult, ":subtract");
		PrintVector3(0, kRowHeight * 2, multiplyResult, ":multiply");
		Novice::ScreenPrintf(0, kRowHeight * 3, "%.2f :dot", dotResult);
		Novice::ScreenPrintf(0, kRowHeight * 4, "%.2f :length", lengthResult);
		PrintVector3(0, kRowHeight * 5, normalizeResult, ":normalize");

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
