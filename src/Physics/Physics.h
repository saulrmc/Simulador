//
// Created by Saúl on 24/01/2026.
//

#ifndef SIMULADORGRAVITACIONAL_PHYSICS_H
#define SIMULADORGRAVITACIONAL_PHYSICS_H
#include "../Commons/CelestialBody.h"
#include "PhysicsUtils.h"

Vec3 force_exerted_from_to(const double m1, const Vec3& position1,
    const double m2, const Vec3& position2);

Vec3 next_velocity_for_delta_time(const double deltaTime, const double m,
    const Vec3& force, const Vec3& current_v);

Vec3 next_position_for_delta_time(const double deltaTime, const Vec3& velocity, const Vec3& current_x) ;

double specific_impact_energy(const double mass1, const double mass2, const Vec3 &relativeVelocity);

double effective_specific_impact_energy(const double mass1, const double mass2, const Vec3 &vel1, const Vec3 &vel2,
    const Vec3& center1, const Vec3& center2);

double mutual_escape_velocity(double mass1, double mass2, Vec3 pos1, Vec3 pos2) ;

double catastrophic_disruption_criterion(Vec3 &vel1, Vec3 &vel2, double radius1, double radius2) ;

#endif //SIMULADORGRAVITACIONAL_PHYSICS_H