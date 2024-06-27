#include "OBB.h"
#include "imgui.h"
#include <Novice.h>
#include <cmath>

OBB::OBB(){}

OBB::~OBB(){}

void OBB::Initialize(Vector3 center, Vector3 rotate, Vector3 size){
    center_ = center;
    color_ = WHITE;
    size_ = size;
    rotation_ = rotate;
    // 回転行列を生成
    Matrix4x4 rotateMat = Matrix4x4::Multiply(Matrix4x4::MakeRotateXMatrix(rotate.x),
                                              Matrix4x4::Multiply(Matrix4x4::MakeRotateYMatrix(rotate.y),
                                              Matrix4x4::MakeRotateZMatrix(rotate.z)));
    //回転行列から軸を抽出
    orientation_[0] = Vector3(rotateMat.m[0][0], rotateMat.m[0][1], rotateMat.m[0][2]);
    orientation_[1] = Vector3(rotateMat.m[1][0], rotateMat.m[1][1], rotateMat.m[1][2]);
    orientation_[2] = Vector3(rotateMat.m[2][0], rotateMat.m[2][1], rotateMat.m[2][2]);
}

void OBB::Update(){
    // 回転行列を生成
    Matrix4x4 rotateMat = Matrix4x4::Multiply(Matrix4x4::MakeRotateXMatrix(rotation_.x),
                                              Matrix4x4::Multiply(Matrix4x4::MakeRotateYMatrix(rotation_.y),
                                              Matrix4x4::MakeRotateZMatrix(rotation_.z)));
    //回転行列から軸を抽出
    orientation_[0] = Vector3(rotateMat.m[0][0], rotateMat.m[0][1], rotateMat.m[0][2]);
    orientation_[1] = Vector3(rotateMat.m[1][0], rotateMat.m[1][1], rotateMat.m[1][2]);
    orientation_[2] = Vector3(rotateMat.m[2][0], rotateMat.m[2][1], rotateMat.m[2][2]);

    //worldMatrix
    matWorld_ = Matrix4x4::MakeAffineMatrix(size_, rotation_, center_);
}

void OBB::Draw(Camera* camera){
    const int kMaxVertices = 8;
    Vector3 vertices[kMaxVertices] = {
        {-size_.x, -size_.y, -size_.z},
        { size_.x, -size_.y, -size_.z},
        { size_.x,  size_.y, -size_.z},
        {-size_.x,  size_.y, -size_.z},
        {-size_.x, -size_.y,  size_.z},
        { size_.x, -size_.y,  size_.z},
        { size_.x,  size_.y,  size_.z},
        {-size_.x,  size_.y,  size_.z}
    };

    Vector3 transformedVertices[kMaxVertices];
    for (int i = 0; i < kMaxVertices; i++){
        transformedVertices[i] = vertices[i].x * orientation_[0] +
            vertices[i].y * orientation_[1] +
            vertices[i].z * orientation_[2];
        transformedVertices[i] += center_;
    }

    Vector3 sc_vertices[kMaxVertices];
    for (int i = 0; i < kMaxVertices; i++){
        Vector3 viewProjTransformed = Matrix4x4::Transform(transformedVertices[i], camera->GetViewProjection());
        sc_vertices[i] = Matrix4x4::Transform(viewProjTransformed, camera->GetViewPort());
    }

    const int edgeIndices[24] = {
        0, 1, 1, 2, 2, 3, 3, 0, 
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7
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

void OBB::UpdateUI(std::string label){
    ImGui::Begin(label.c_str());
    ImGui::DragFloat3("Center", &center_.x, 0.01f);
    ImGui::DragFloat3("Rotation", &rotation_.x, 0.01f); // Rotationを追加してUIで操作可能に
    ImGui::DragFloat3("Size", &size_.x, 0.01f); // Sizeも追加
    ImGui::End();
}

Vector3 OBB::GetCenter() const{ return center_; }
Vector3* OBB::GetOrientation() const{ return const_cast< Vector3* >(orientation_); }
Vector3 OBB::GetSize() const{ return size_; }
Matrix4x4 OBB::GetMatWorld()const{ return matWorld_; }

void OBB::SetCenter(const Vector3 center){ center_ = center; }
void OBB::SetOrientation(const Vector3* orientation){
    orientation_[0] = orientation[0];
    orientation_[1] = orientation[1];
    orientation_[2] = orientation[2];
}
void OBB::SetSize(const Vector3 size){ size_ = size; }

uint32_t OBB::GetColor() const{ return color_; }
void OBB::SetColor(const uint32_t color){ color_ = color; }
