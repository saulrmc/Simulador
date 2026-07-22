//
// Created by Saúl on 15/02/2026.
//

#pragma once
#ifndef SIMULADORGRAVITACIONAL_COLLISIONS_H
#define SIMULADORGRAVITACIONAL_COLLISIONS_H
#include <vector>
#include "CelestialBodies.h"
#include "spatial_structures/geometry.h"
#include "../Creation/Creation.h"

static constexpr double DENSITY = 9511.09;
static constexpr double MI = 0.5;

void resolve_collision(CelestialBodies &bodies, int body1Index, int body2Index);

template<typename Structure>
int collisions(Structure* structure, CelestialBodies& bodies) {
    for (int i = 0; i < bodies.size(); i++) {
        structure->query_region(
            [](CelestialBodies& b, int i1, int i2) {
                resolve_collision(b, i1, i2);
            }, bodies, i);
    }
    return bodies.apply_deletions();
}

void resolve_collision(CelestialBodies &bodies, int body1Index, int body2Index);
void simplified_resolve_collision(CelestialBodies &bodies, int body1Index, int body2Index);

void merge_regime(CelestialBodies &bodies,
    int largestBodyIndex, int smallestBodyIndex);
void super_catastrophic_disruption_regime(CelestialBodies &bodies,
    int largestBodyIndex, int smallestBodyIndex, double disruptionEnergy);
void disruption_regime(CelestialBodies &bodies,
    int largestBodyIndex, int smallestBodyIndex, double disruptionEnergy);
void hit_and_run_regime(CelestialBodies &bodies,
    int largestBodyIndex, int smallestBodyIndex, double massInteract);


//Cálculos para distinguishir regímenes:

double mutual_escape_velocity(double mass1, double mass2, const Vec3 &pos1, const Vec3 &pos2);
double mutual_escape_velocity_mod(double largestMass, double massInteract, double avgDensity);
double catastrophic_disruption_criterion(const Vec3 &vel1, const Vec3 &vel2, double combinedRadius,
    double avgDensity);
double collision_timescale(double radius1, double radius2, double distance, double relativeVelocity);
double collision_angle(const Vec3 &vel1, const Vec3 &vel2, const Vec3& center1, const Vec3& center2);
double mass_interact(CelestialBodies &bodies, int largestBodyIndex, int smallestBodyIndex);
double disruption_curve(double combinedRadius, double avgDensity);
double critical_impact_velocity_mod(double combinedRadius, double avgDensity);
double disruption_criterion(double disruptionCurve, double relationMass);
double critical_impact_velocity(double criticalImpVelMod, double relationMass);
double disruption_energy_by_angle(double disruptionCriterion, double reducedMass, double reducedMassMod);
double specific_impact_energy(double massLR, double disruptionEnergy, double largestMass, double smallestMass);
double specific_impact_energy(double reducedMass, double impactVelocity, double totalMass);
double impact_velocity(double reducedMass, double specificImpEnergy, double totalMass);
double mass_largest_remnant(double specificImpEnergyErosion, double disruptionEnergy, double totalMass);
double mass_second_largest_remnant(double mLR, double totalMass, int N1, int N2);
Vec3 momentum(const Vec3 &vel1, double mass1, const Vec3 &vel2, double mass2);
Vec3 velCM(const Vec3 &vel1, double mass1, const Vec3 &vel2, double mass2);
double radius_by_density_and_mass(double mass, double density);
double density_by_mass_and_radius(double mass, double radius);
void refresh_body(CelestialBodies &bodies, int indexBody, double newMass,
    const Vec3 &newVelocity);
void fix_positions_after_fragmentation(CelestialBodies &bodies,
    int largestBodyIndex, int smallestBodyIndex);
double mass_largest_remnant_supcat(double specificImpEnergySC, double disruptionEnergy,
    double totalMass);
void compute_remnant_properties_and_velocities(CelestialBodies &bodies,
    int largestBodyIndex, int smallestBodyIndex, double mLR, int N1, int N2);
#endif //SIMULADORGRAVITACIONAL_COLLISIONS_H
