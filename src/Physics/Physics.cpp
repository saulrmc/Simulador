//
// Created by Saúl on 8/01/2026.
//
#include "../Commons/CelestialBody.h"
#include "PhysicsUtils.h"
static constexpr double EPSILON = 1e-4;

Vec3 force_exerted_from_to(const double m1, const Vec3& position1, const double m2, const Vec3& position2) {
    //F=G*m1*m2/r^2
    const Vec3 r = position2 - position1;
    const double dist = r.magnitude() + EPSILON;
    return r*(-units::G*(m1*m2)/(dist*dist*dist));
}
