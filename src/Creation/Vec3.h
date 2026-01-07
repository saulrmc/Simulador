//
// Created by Saúl on 6/01/2026.
//

#ifndef SIMULADORGRAVITACIONAL_VEC3_H
#define SIMULADORGRAVITACIONAL_VEC3_H
#include <cmath>
#include <iostream>

class Vec3 {
    public:
    Vec3();
    Vec3(double x, double y, double z);
    ~Vec3();

    Vec3 operator-(const Vec3& v) const;
    Vec3 operator+(const Vec3& v) const;
    Vec3 operator*(double scalar) const;
    Vec3 operator/(double scalar) const;
    double dot(const Vec3& v) const;
    double magnitude() const;
    Vec3 normalize() const;

    private:
    double x, y, z;

};


#endif //SIMULADORGRAVITACIONAL_VEC3_H