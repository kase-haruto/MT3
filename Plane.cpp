#include "Plane.h"
#include"Matrix4x4.h"
#include"MyFunc.h"
#include<Novice.h>
#include<stdint.h>
#include"imgui.h"

Plane::Plane(){}

Plane::~Plane(){}

void Plane::Initialze(){
	normal_ = {0.0f,1.0f,0.0f};
	distance_ = 1.0f;
}

void Plane::UpdateImGui(){
	ImGui::Begin("plane");
	ImGui::DragFloat3("normal", &normal_.x, 0.01f);
	normal_ = Vector3::Normalize(normal_);
	ImGui::DragFloat("distance", &distance_, 0.01f);
	ImGui::End();
}

void Plane::Draw(const Camera* camera, uint32_t color){
	Vector3 center;
	center.x = normal_.x * distance_;//中心点を決める
	center.y = normal_.y * distance_;//中心点を決める
	center.z = normal_.z * distance_;//中心点を決める
	Vector3 perpendiculars[4];
	perpendiculars[0] = Vector3::Normalize(Perpendicular(normal_));//法線と垂直なベクトルを一つ求める
	perpendiculars[1] = {-perpendiculars[0].x,-perpendiculars[0].y,-perpendiculars[0].z};//求めたベクトルの逆ベクトルを求める
	perpendiculars[2] = Cross(normal_, perpendiculars[0]);//法線とクロス積を求める
	perpendiculars[3] = {-perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z};//求めたベクトルの逆ベクトルを求める
	//４頂点を求める
	Vector3 points[4];
	for (int32_t index = 0; index < 4; index++){
		Vector3 extend;
		extend.x= perpendiculars[index].x * 2.0f;
		extend.y= perpendiculars[index].y * 2.0f;
		extend.z= perpendiculars[index].z * 2.0f;
		Vector3 point = center + extend;
		points[index] = Matrix4x4::Transform(Matrix4x4::Transform(point, camera->GetViewProjection()), camera->GetViewPort());
	}

	//板の描画
	Novice::DrawLine(static_cast< int >(points[0].x), static_cast< int >(points[0].y),
					 static_cast< int >(points[2].x), static_cast< int >(points[2].y), color);
	Novice::DrawLine(static_cast< int >(points[1].x), static_cast< int >(points[1].y),
					 static_cast< int >(points[2].x), static_cast< int >(points[2].y), color);
	Novice::DrawLine(static_cast< int >(points[1].x), static_cast< int >(points[1].y),
					 static_cast< int >(points[3].x), static_cast< int >(points[3].y), color);
	Novice::DrawLine(static_cast< int >(points[3].x), static_cast< int >(points[3].y),
					 static_cast< int >(points[0].x), static_cast< int >(points[0].y), color);
}