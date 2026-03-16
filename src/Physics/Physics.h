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

// double specific_impact_energy(const double mass1, const double mass2, const Vec3 &relativeVelocity);

// double effective_specific_impact_energy(const double mass1, const double mass2, const Vec3 &vel1, const Vec3 &vel2,
//     const Vec3& center1, const Vec3& center2);

double mutual_escape_velocity(double mass1, double mass2, Vec3 pos1, Vec3 pos2) ;

double mutual_escape_velocity_mod(double largestMass, double massInteract) ;

double catastrophic_disruption_criterion(const Vec3 &vel1, const Vec3 &vel2, double combinedRadius) ;

double collision_timescale(double radius1, double radius2, double distance, double relativeVelocity) ;

double collision_angle(const Vec3 &vel1, const Vec3 &vel2, const Vec3& center1, const Vec3& center2);

double mass_interact(const Vec3 &vel1, const Vec3 &vel2, const Vec3& center1, const Vec3& center2, double radius2);

double disruption_curve(double radius1, double radius2);

double critical_impact_velocity_mod(double combinedRadius);

double disruption_criterion(double disruptionCurve, double relationMass);

double critical_impact_velocity(double criticalImpVelMod, double relationMass);

double disruption_energy_by_angle(double disruptionCriterion, double reducedMass, double reducedMassMod);

double critical_impact_velocity_by_angle(double disruptionEnergy, double totalMass, double reducedMass);

double specific_impact_energy(double massLR, double disruptionEnergy, double largestMass, double smallestMass);

double impact_velocity(double reducedMass, double specificImpEnergy, double totalMass) ;
#endif //SIMULADORGRAVITACIONAL_PHYSICS_H