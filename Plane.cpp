//#include "Plane.h"
//#include"Matrix4x4.h"
//#include"MyFunc.h"
//#include<Novice.h>
//#include<stdint.h>
//
//Plane::Plane(){}
//
//Plane::~Plane(){}
//
//void Plane::Initialze(){
//	normal_ = Vector3::Normalize(normal_);
//	distance_ = 0;
//}
//
//void Plane::Update(){
//
//}
//
//void Plane::Draw(const Camera* camera){
//	Vector3 center = normal_ * distance_;
//	Vector3 perpendiculars[4];
//	perpendiculars[0] = Vector3::Normalize(normal_);
//	perpendiculars[1] = {-perpendiculars[0].x,-perpendiculars[0].y,-perpendiculars[0].z};
//	perpendiculars[2] = Cross(normal_, perpendiculars[0]);
//	perpendiculars[3] = {-perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z};
//
//	Vector3 pos[4];
//	for (int32_t index = 0; index < 4; index++){
//		Vector3 extend = perpendiculars[index] * 2.0f;
//		Vector3 point = center + extend;
//		pos[index] = Matrix4x4::Transform(Matrix4x4::Transform(point, camera->GetViewProjection()), camera->GetViewPort());
//		Novice::DrawLine(static_cast< int >(pos[index].x), static_cast< int >(pos[index].y),
//						 static_cast< int >(pos[(index + 1) % 4].x), static_cast< int >(pos[(index + 1) % 4].y),
//						 WHITE
//		);
//	}
//
//}