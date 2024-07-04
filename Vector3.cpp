#include"Vector3.h"
#include<cmath>

float Vector3::Length(const Vector3& v) {
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Vector3::Normalize(const Vector3& v) {
	float length = Length(v);
	if (length != 0.0f) {
		return { v.x / length, v.y / length, v.z / length };
	} else {
		return { 0.0f, 0.0f, 0.0f }; // ゼロベクトルを返す
	}
}

float Vector3::Norm(Vector3 v) {
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Vector3::Min(const Vector3& a, const Vector3& b){
	return Vector3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}

Vector3 Vector3::Max(const Vector3& a, const Vector3& b){
	return Vector3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}




//除算
Vector3 Vector3::operator/(const float& scalar) const {
	float newX = x / scalar;
	float newY = y / scalar;
	float newZ = z / scalar;
	return Vector3(newX, newY, newZ);
}

Vector3 Vector3::operator/=(const float& scalar) {
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return Vector3(x, y, z);
}

Vector3 Vector3::operator/(const Vector3& other) const {
	float newX = x / other.x;
	float newY = y / other.y;
	float newZ = z / other.z;
	return Vector3(newX, newY, newZ);
}

Vector3 Vector3::operator/=(const Vector3& other) {
	x /= other.x;
	y /= other.y;
	z /= other.z;
	return Vector3(x, y, z);
}

Vector3 operator/(float scalar, const Vector3& vec){
	return Vector3(vec.x / scalar, vec.y / scalar, vec.z / scalar);
}

// ベクトルの加算
Vector3 Vector3::operator+(const float& scalar) const {
	float newX = x + scalar;
	float newY = y + scalar;
	float newZ = z + scalar;
	return Vector3(newX, newY, newZ);
}

Vector3 Vector3::operator+=(const float& scalar) {
	x += scalar;
	y += scalar;
	z += scalar;
	return Vector3(x, y, z);
}

Vector3 Vector3::operator+(const Vector3& other) const {
	float newX = x + other.x;
	float newY = y + other.y;
	float newZ = z + other.z;
	return Vector3(newX, newY, newZ);
}

Vector3 Vector3::operator+=(const Vector3& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return Vector3(x, y, z);
}

Vector3 operator+(float scalar, const Vector3& vec){
	return Vector3(vec.x + scalar, vec.y + scalar, vec.z + scalar);
}

// ベクトルの減算
Vector3 Vector3::operator-(const float& scalar) const {
	float newX = x - scalar;
	float newY = y - scalar;
	float newZ = z - scalar;
	return Vector3(newX, newY, newZ);
}


Vector3 Vector3::operator-=(const float& scalar) {
	x -= scalar;
	y -= scalar;
	z -= scalar;
	return Vector3(x, y, z);
}

Vector3 Vector3::operator-(const Vector3& other) const {
	float newX = x - other.x;
	float newY = y - other.y;
	float newZ = z - other.z;
	return Vector3(newX, newY, newZ);
}

Vector3 Vector3::operator-=(const Vector3& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return Vector3(x, y, z);
}

Vector3 operator-(float scalar, const Vector3& vec){
	return Vector3(vec.x - scalar, vec.y - scalar, vec.z - scalar);
}