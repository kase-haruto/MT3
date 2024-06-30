#include "Ball.h"
#include"MyStruct.h"

Ball::Ball(){}

Ball::~Ball(){}

void Ball::Initialize(const Vector3& center,const float& radius, const float& mass, const uint32_t& color){
	sphere_ = std::make_unique<Sphere>();
	sphere_->Init(center, {0.0f,0.0f,0.0f}, radius, color);
	velocity_ = {0.0f,0.0f,0.0f};
	mass_ = mass;
}

void Ball::Update(){
    // 摩擦係数を定義（例として適当な値を設定）
    float frictionCoefficient = 0.005f;

    // 空気抵抗による減速を計算
    Vector3 airResistance = velocity_ *-airResistanceCoefficient * Vector3::Length(velocity_);

    // 摩擦による減速を計算
    Vector3 friction = velocity_ *-frictionCoefficient;

    // 速度に空気抵抗と摩擦を加える
    velocity_ += airResistance + friction;

    // 位置を更新
    sphere_->SetCenter(sphere_->GetCenter() + velocity_);
}

void Ball::Draw(Camera* cam){
	sphere_->Draw(cam);
}