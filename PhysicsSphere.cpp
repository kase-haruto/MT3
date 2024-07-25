#include "PhysicsSphere.h"
#include"imgui.h"
#include"MyFunc.h"
PhysicsSphere::PhysicsSphere(){

}

PhysicsSphere::~PhysicsSphere(){}

void PhysicsSphere::Initialize(Vector3 center, float radius, uint32_t color){
	Sphere::Init(center, radius, color);
	acceleration_ = {0.0f,0.0f,0.0f};
	velocity_ = {0.0f,0.0f,0.0f};
	mass_ = 0.0f;
}


void PhysicsSphere::Update(){
	velocity_ += acceleration_ * deltaTime;
	transform_.translation += velocity_ * deltaTime;
}


void PhysicsSphere::Draw(Camera* camera){
	Sphere::Draw(camera);
}

void PhysicsSphere::Debug(){
	ImGui::Begin("ball");
	ImGui::DragFloat3("velocity", &velocity_.x, 0.01f);
	ImGui::DragFloat3("acceleration", &acceleration_.x, 0.01f);
	ImGui::End();
}