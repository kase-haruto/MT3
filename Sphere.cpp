#include "Sphere.h"
#define _USE_MATH_DEFINES
#include<cmath>
#include<Novice.h>

#include<imgui.h>

void Sphere::Init(Vector3 center, Vector3 rotate, float radius, uint32_t color) {
	transform_.translation = center;
	transform_.rotation = rotate;
	radius_ = radius;
	color_ = color;
	worldMatrix_ = Matrix4x4::MakeAffineMatrix({ 1.0f,1.0f,1.0f }, transform_.rotation, transform_.translation);
}

void Sphere::UpdateImGui(const char* lavel) {
	ImGui::Begin(lavel);
	ImGui::DragFloat3("translate", &transform_.translation.x, 0.01f);
	ImGui::DragFloat("radius", &radius_, 0.01f);
	ImGui::End();
}

void Sphere::Draw(const Camera* cam) {
	// 分割数
	const uint32_t kSubdivision = 8; // 任意の適切な値を設定
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
			a.x = radius_ * (std::cos(lat) * std::cos(lon)) + transform_.translation.x;
			a.y = radius_ * std::sin(lat) + transform_.translation.y;
			a.z = radius_ * (std::cos(lat) * std::sin(lon)) + transform_.translation.z;

			b.x = radius_ * (std::cos(lat + kLatEvery) * std::cos(lon)) + transform_.translation.x;
			b.y = radius_ * std::sin(lat + kLatEvery) + transform_.translation.y;
			b.z = radius_ * (std::cos(lat + kLatEvery) * std::sin(lon)) + transform_.translation.z;

			c.x = radius_ * (std::cos(lat) * std::cos(lon + kLonEvery)) + transform_.translation.x;
			c.y = radius_ * std::sin(lat) + transform_.translation.y;
			c.z = radius_ * (std::cos(lat) * std::sin(lon + kLonEvery)) + transform_.translation.z;

			//スクリーン座標まで変換させる
			ndcPosA = Matrix4x4::Transform(a,cam->GetViewProjection());
			ndcPosB = Matrix4x4::Transform(b,cam->GetViewProjection());
			ndcPosC = Matrix4x4::Transform(c,cam->GetViewProjection());

			screenPosA = Matrix4x4::Transform(ndcPosA, cam->GetViewPort());
			screenPosB = Matrix4x4::Transform(ndcPosB, cam->GetViewPort());
			screenPosC = Matrix4x4::Transform(ndcPosC, cam->GetViewPort());

			//ab,bcで線を引く
			Novice::DrawLine(int(screenPosA.x), int(screenPosA.y), int(screenPosB.x), int(screenPosB.y), color_);
			Novice::DrawLine(int(screenPosA.x), int(screenPosA.y), int(screenPosC.x), int(screenPosC.y), color_);
		}
	}
	preCenter_ = GetCenter();
}

void Sphere::SetCenter(const Vector3& center) { transform_.translation = center; }

float Sphere::GetRadius()const { return radius_; }

void Sphere::SetColor(const uint32_t color){ color_ = color; }

Vector3 Sphere::GetCenter()const{
	Vector3 center;
	center = transform_.translation;
	return center;
}

Vector3 Sphere::GetPreCenter()const{return preCenter_;}