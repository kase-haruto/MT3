#pragma once
#include"Vector3.h"
#include<cmath>

class Quaternion{
public:
    float w, x, y, z;

    Quaternion() : w(1), x(0), y(0), z(0){}
    Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z){}

    static Quaternion RotationAxis(const Vector3& axis, float angle){
        float halfAngle = angle * 0.5f;
        float s = std::sin(halfAngle);
        return Quaternion(std::cos(halfAngle), axis.x * s, axis.y * s, axis.z * s);
    }

    Vector3 Rotate(const Vector3& v) const{
        Quaternion qv(0, v.x, v.y, v.z);
        Quaternion qConj = Conjugate();
        Quaternion qResult = *this * qv * qConj;
        return Vector3(qResult.x, qResult.y, qResult.z);
    }

    Quaternion Conjugate() const{
        return Quaternion(w, -x, -y, -z);
    }

    Quaternion operator*(const Quaternion& q) const{
        return Quaternion(
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w
        );
    }
};
