#include "Camera.h"
#include<cmath>
#include"imgui.h"

Camera* Camera::GetInstance(){
	static Camera instance;
	return&instance;
}

Camera::Camera(){

}

Camera::~Camera(){

}

void Camera::Initialize(){
	matProjection_ = MakePerspectiveFovMatrix(0.45f, 1280.0f / 720.0f, 0.1f, 100.0f);
	matViewPort_ = MakeViewportMatrix(0, 0, 1280.0f, 720.0f, 0.0f, 1.0f);
	matViewProjection_ = Matrix4x4::Multiply(matView_, matProjection_);
}


void Camera::Update(){
	ImGui::Begin("camera");
	ImGui::DragFloat3("translate", &translate_.x, 0.01f);
	ImGui::DragFloat3("rotate", &rotate_.x, 0.01f);
	ImGui::End();

	cameraMatrix_ = Matrix4x4::MakeAffineMatrix({1.0f,1.0f,1.0f}, rotate_, translate_);
	matView_ = Matrix4x4::Inverse(cameraMatrix_);
	matViewProjection_ = Matrix4x4::Multiply(matView_, matProjection_);
}

Matrix4x4 Camera::MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip){
	Matrix4x4 result;
	result = {
		1 / aspectRatio * 1 / std::tan(fovY / 2),0,0,0,
		0,1 / std::tan(fovY / 2),0,0,
		0,0,farClip / (farClip - nearClip),1,
		0,0,-nearClip * farClip / (farClip - nearClip),0
	};
	return result;
}

Matrix4x4 Camera::MakeViewportMatrix(float l, float t, float w, float h, float minDepth, float maxDepth){
	Matrix4x4 result;
	result = {
		w / 2,0,0,0,
		0,-h / 2,0,0,
		0,0,maxDepth - minDepth,0,
		l + (w / 2),t + (h / 2),minDepth,1
	};
	return result;
}