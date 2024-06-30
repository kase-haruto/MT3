#include"MyFunc.h"
#include<algorithm>
/// <summary>
/// ベクトルの内積を求めます
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns>ベクトルの内積</returns>
float Dot(const Vector3& v1, const Vector3& v2){
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

/// <summary>
/// 正射影ベクトルを求めます
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns>計算結果を返します</returns>
Vector3 Projection(const Vector3& v1, const Vector3& v2){
	Vector3 b = Vector3::Normalize(v2);
	float d = Dot(v1, b);
	return b * d;
}

/// <summary>
/// 最近接点を求めます
/// </summary>
/// <param name="point"></param>
/// <param name="segment"></param>
/// <returns>最近接点の値を返します</returns>
Vector3 ClosestPoint(const Vector3& point, const Segment& segment){
	return segment.origin + Projection((point - segment.origin), segment.diff);
}

/// <summary>
/// 垂直ベクトルをもとめる
/// </summary>
/// <param name="v"></param>
/// <returns>垂直ベクトル</returns>
Vector3 Perpendicular(const Vector3& v){
	if (v.x != 0.0f || v.y != 0.0f){
		return {-v.y, v.x, 0.0f};
	}
	return {0.0f, -v.z, v.y};
}

/// <summary>
///	クロス積
/// </summary>
/// <returns>計算結果</returns>
Vector3 Cross(const Vector3& v1, const Vector3& v2){
	Vector3 result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

Matrix4x4 MakeRotateAxisMatrix(const Vector3& axis, float angle){
	Matrix4x4 result;
	float cosAngle = std::cos(angle);
	float sinAngle = std::sin(angle);
	float oneMinusCos = 1.0f - cosAngle;

	result.m[0][0] = cosAngle + axis.x * axis.x * oneMinusCos;
	result.m[0][1] = axis.x * axis.y * oneMinusCos - axis.z * sinAngle;
	result.m[0][2] = axis.x * axis.z * oneMinusCos + axis.y * sinAngle;
	result.m[0][3] = 0.0f;

	result.m[1][0] = axis.y * axis.x * oneMinusCos + axis.z * sinAngle;
	result.m[1][1] = cosAngle + axis.y * axis.y * oneMinusCos;
	result.m[1][2] = axis.y * axis.z * oneMinusCos - axis.x * sinAngle;
	result.m[1][3] = 0.0f;

	result.m[2][0] = axis.z * axis.x * oneMinusCos - axis.y * sinAngle;
	result.m[2][1] = axis.z * axis.y * oneMinusCos + axis.x * sinAngle;
	result.m[2][2] = cosAngle + axis.z * axis.z * oneMinusCos;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

Vector3 ExtractEulerAngles(const Matrix4x4& matrix){
	Vector3 euler;

	if (matrix.m[2][0] < 1.0f){
		if (matrix.m[2][0] > -1.0f){
			euler.y = std::asin(matrix.m[2][0]);
			euler.x = std::atan2(-matrix.m[2][1], matrix.m[2][2]);
			euler.z = std::atan2(-matrix.m[1][0], matrix.m[0][0]);
		} else{
			euler.y = -static_cast< int >(std::numbers::pi) / 2.0f;
			euler.x = -std::atan2(matrix.m[0][1], matrix.m[1][1]);
			euler.z = 0.0f;
		}
	} else{
		euler.y = static_cast< int >(std::numbers::pi) / 2.0f;
		euler.x = std::atan2(matrix.m[0][1], matrix.m[1][1]);
		euler.z = 0.0f;
	}

	return euler;
}