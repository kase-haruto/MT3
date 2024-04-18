﻿#include"Vector3.h"

Vector3 Vector3::operator*(const float& scalar) const {
	return { x * scalar,y * scalar };
}

Vector3& Vector3::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	return *this;
}

Vector3& Vector3::operator/=(float divisor) {
	x /= divisor;
	y /= divisor;
	return *this;
}


// ベクトルの加算
Vector3 Vector3::operator+(const Vector3& other) const {
	return { x + other.x, y + other.y };
}

Vector3 Vector3::operator+=(const Vector3& other) {
	this->x += other.x;
	this->y += other.y;
	return *this;
}

// ベクトルの減算
Vector3 Vector3::operator-(const Vector3& other) const {
	return { x - other.x, y - other.y };
}

Vector3 Vector3::operator-=(const Vector3& other) {
	this->x -= other.x;
	this->y -= other.y;
	return *this;
}

//ベクトルの乗算
Vector3 Vector3::operator*(const Vector3& other) const {
	return { x * other.x, y * other.y };
}

Vector3 Vector3::operator*=(const Vector3& other) {
	this->x *= other.x;
	this->y *= other.y;
	return *this;
}