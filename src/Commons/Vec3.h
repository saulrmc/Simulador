//
// Created by Saúl on 6/01/2026.
//
#pragma once
#ifndef SIMULADORGRAVITACIONAL_VEC3_H
#define SIMULADORGRAVITACIONAL_VEC3_H
#include <cmath>

class Vec3 {
    public:
    Vec3();
    Vec3(double x, double y, double z);
    ~Vec3();

    Vec3 operator-(const Vec3& v) const;
    Vec3 operator+(const Vec3& v) const;
    Vec3 operator*(double scalar) const;
    Vec3 operator/(double scalar) const;
    Vec3 operator/(const Vec3 &v) const;
    double dot(const Vec3& v) const;
    double magnitude() const;
    Vec3 normalize() const;
    double distance(const Vec3& v) const;
    double get_x() const;
    double get_y() const;
    double get_z() const;
    void set_x(const double x);
    void set_y(const double y);
    void set_z(const double z);

    private:
    double x, y, z;

};
Vec3 operator/(double scalar, const Vec3& v);


inline Vec3::Vec3() {
    this->x=0;
    this->y=0;
    this->z=0;
}

inline Vec3::Vec3(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

inline Vec3::~Vec3() {
}

inline Vec3 Vec3::operator-(const Vec3 &v) const {
    return Vec3(x - v.x, y - v.y, z - v.z);
}

inline Vec3 Vec3::operator+(const Vec3 &v) const {
    return Vec3(x + v.x, y + v.y, z + v.z);
}

inline Vec3 Vec3::operator*(double scalar) const {
    return Vec3(x * scalar, y * scalar, z * scalar);
}

inline Vec3 Vec3::operator/(double scalar) const {
    return Vec3(x / scalar, y / scalar, z / scalar);
}

inline Vec3 Vec3::operator/(const Vec3 &v) const {
    return Vec3(x / v.x, y / v.y, z / v.z);
}

inline double Vec3::dot(const Vec3 &v) const {
    return x * v.x + y * v.y + z * v.z;
}

inline double Vec3::magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
}

inline Vec3 Vec3::normalize() const {
    double mag = magnitude();
    return Vec3(x / mag, y / mag, z / mag);
}

inline double Vec3::distance(const Vec3 &v) const {
    double dx = x - v.x;
    double dy = y - v.y;
    double dz = z - v.z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}

inline double Vec3::get_x() const {
    return x;
}

inline double Vec3::get_y() const {
    return y;
}

inline double Vec3::get_z() const {
    return z;
}

inline void Vec3::set_x(const double x) {
    this->x = x;
}

inline void Vec3::set_y(const double y) {
    this->y = y;
}

inline void Vec3::set_z(const double z) {
    this->z = z;
}

inline Vec3 operator/(double scalar, const Vec3 &v) {
    return Vec3(scalar / v.get_x(), scalar / v.get_y(), scalar / v.get_z());
}


#endif //SIMULADORGRAVITACIONAL_VEC3_H