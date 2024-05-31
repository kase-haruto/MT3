#include "AABB.h"
#include"imgui.h"
#include<Novice.h>
#include<cmath>
AABB::AABB(){}

AABB::~AABB(){}

void AABB::Initialize(Vector3 min, Vector3 max, uint32_t color){
	min_ = min;
	max_ = max;
	color_ = color;
}

void AABB::Update(){
    //minとmaxが入れ替わらないようにする
    min_.x = ( std::min ) (min_.x, max_.x);
    max_.x = ( std::max ) (min_.x, max_.x);

    min_.y = ( std::min ) (min_.y, max_.y);
    max_.y = ( std::max ) (min_.y, max_.y);

    min_.z = ( std::min ) (min_.z, max_.z);
    max_.z = ( std::max ) (min_.z, max_.z);
}

void AABB::Draw(Camera* camera){
    const int kMaxVertices = 8;
    Vector3 vertices[kMaxVertices] = {
        {min_.x, min_.y, min_.z},
        {max_.x, min_.y, min_.z},
        {max_.x, max_.y, min_.z},
        {min_.x, max_.y, min_.z},
        {min_.x, min_.y, max_.z},
        {max_.x, min_.y, max_.z},
        {max_.x, max_.y, max_.z},
        {min_.x, max_.y, max_.z}
    };

    // 座標系変換
    Vector3 sc_vertices[kMaxVertices];
    for (int i = 0; i < kMaxVertices; i++){
        // ビュー変換とプロジェクション変換
        Vector3 viewProjTransformed = Matrix4x4::Transform(vertices[i], camera->GetViewProjection());
        // ビューポート変換
        sc_vertices[i] = Matrix4x4::Transform(viewProjTransformed, camera->GetViewPort());
    }

    // 描画するエッジのインデックス
    const int edgeIndices[24] = {
        0, 1, 1, 2, 2, 3, 3, 0, // 底面
        4, 5, 5, 6, 6, 7, 7, 4, // 上面
        0, 4, 1, 5, 2, 6, 3, 7  // 側面
    };

    for (int i = 0; i < 24; i += 2){
        Novice::DrawLine(
            static_cast< int >(sc_vertices[edgeIndices[i]].x),
            static_cast< int >(sc_vertices[edgeIndices[i]].y),
            static_cast< int >(sc_vertices[edgeIndices[i + 1]].x),
            static_cast< int >(sc_vertices[edgeIndices[i + 1]].y),
            color_
        );
    }
}

void AABB::UpdateUI(std::string lavel){
	ImGui::Begin(lavel.c_str());
	ImGui::DragFloat3("min", &min_.x, 0.01f);
	ImGui::DragFloat3("max", &max_.x, 0.01f);
	ImGui::End();
}

Vector3 AABB::GetMin()const{ return min_; }
Vector3 AABB::GetMax()const{ return max_; }

void AABB::SetMin(const Vector3 min){ min_ = min; }
void AABB::SetMax(const Vector3 max){ max_ = max; }

uint32_t AABB::GetColor()const{ return color_; }
void AABB::SetColor(const uint32_t color){ color_ = color; }