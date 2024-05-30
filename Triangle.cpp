#include "Triangle.h"
#include"Novice.h"
#include"Matrix4x4.h"
#include"imgui.h"

Triangle::Triangle(){}
Triangle::~Triangle(){}

void Triangle::Initialize(uint32_t color){
	color_ = color;
}

void Triangle::UiUpdate(){
	ImGui::Begin("Triangle");
	ImGui::DragFloat3("top",&vertices_[1].x,0.01f);
	ImGui::DragFloat3("RBottom",&vertices_[2].x,0.01f);
	ImGui::DragFloat3("LBottom",&vertices_[0].x,0.01f);
	ImGui::End();
}

void Triangle::Draw(Camera* camera){
	const int kMaxIndex = 3;
	Vector3 sc_v[3];

	///===========================================
	///	座標変換
	for (int index = 0; index < kMaxIndex; index++){
		sc_v[index] = Matrix4x4::Transform(Matrix4x4::Transform(vertices_[index], camera->GetViewProjection()), camera->GetViewPort());

	}

	///===========================================
	///	描画
	Novice::DrawTriangle(static_cast<int>(sc_v[0].x),static_cast<int>(sc_v[0].y),
						 static_cast<int>(sc_v[1].x),static_cast<int>(sc_v[1].y),
						 static_cast<int>(sc_v[2].x),static_cast<int>(sc_v[2].y),
						 color_,
						 kFillModeWireFrame);
}

///==========================================
///		アクセッサ
///==========================================
Vector3 Triangle::GetLBottom()const{ return vertices_[0]; }
Vector3 Triangle::GetTop()const{ return vertices_[1]; }
Vector3 Triangle::GetRBottom()const{ return vertices_[2]; }

void Triangle::SetLBottom(const Vector3 vertices){ vertices_[0] = vertices; }
void Triangle::SetTop(const Vector3 vertices){ vertices_[1] = vertices; }
void Triangle::SetRBottom(const Vector3 vertices){ vertices_[2] = vertices; }
