//
// Created by Saúl on 24/01/2026.
//

#ifndef SIMULADORGRAVITACIONAL_PHYSICS_H
#define SIMULADORGRAVITACIONAL_PHYSICS_H
Vec3 force_exerted_from_to(const double m1, const Vec3& position1,
    const double m2, const Vec3& position2);

Vec3 next_velocity_for_delta_time(const double delta_time, const double m,
    const Vec3& force, const Vec3& current_v);

Vec3 next_position_for_delta_time(const double delta_time, const Vec3& velocity, const Vec3& current_x) ;


#endif //SIMULADORGRAVITACIONAL_PHYSICS_H