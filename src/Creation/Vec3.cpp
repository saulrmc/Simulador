//
// Created by Saúl on 6/01/2026.
//

#include "../Creation/Vec3.h"

Vec3::Vec3() {
    this->x=0;
    this->y=0;
    this->z=0;
}

Vec3::Vec3(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vec3::~Vec3() {
}

Vec3 Vec3::operator-(const Vec3 &v) const {
    return Vec3(x - v.x, y - v.y, z - v.z);
}

Vec3 Vec3::operator+(const Vec3 &v) const {
    return Vec3(x + v.x, y + v.y, z + v.z);
}

Vec3 Vec3::operator*(double scalar) const {
    return Vec3(x * scalar, y * scalar, z * scalar);
}

Vec3 Vec3::operator/(double scalar) const {
    return Vec3(x / scalar, y / scalar, z / scalar);
}

double Vec3::dot(const Vec3 &v) const {
    return x * v.x + y * v.y + z * v.z;
}

double Vec3::magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
}

Vec3 Vec3::normalize() const {
    double mag = magnitude();
    return Vec3(x / mag, y / mag, z / mag);
}


