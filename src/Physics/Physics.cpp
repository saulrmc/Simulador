//
// Created by Saúl on 8/01/2026.
//
#include "../Commons/CelestialBody.h"
#include "PhysicsUtils.h"
static constexpr double EPSILON = 1e-4;


//cuando se usen estas funciones se debe manejar correctamente el uso de dt y dt/2
//para usar leapfrog kdk o leapfrog puro

Vec3 force_exerted_from_to(const double m1, const Vec3& position1,
    const double m2, const Vec3& position2) {
    //F=G*m1*m2/r^2
    const Vec3 r = position2 - position1;
    const double dist = r.magnitude() + EPSILON;
    return r*(-units::G*(m1*m2)/(dist*dist*dist));
}

Vec3 next_velocity_for_delta_time(const double delta_time, const double m,
    const Vec3& force, const Vec3& current_v) { //delta time debería ser dt/2 pero eso no le corresponde a esta funcion
    //v(t + dt/2) = v(t) + (F(t)/m) * dt
    return Vec3(current_v + (force/m)*delta_time);
}

Vec3 next_position_for_delta_time(const double delta_time, const Vec3& velocity, const Vec3& current_x) {
    //x(t + dt) = x(t) + v(t + dt/2) * dt
    return Vec3(current_x + velocity*delta_time);
}