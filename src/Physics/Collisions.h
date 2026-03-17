//
// Created by Saúl on 15/02/2026.
//

#ifndef SIMULADORGRAVITACIONAL_COLLISIONS_H
#define SIMULADORGRAVITACIONAL_COLLISIONS_H
#include <vector>
#include "../Commons/CelestialBody.h"
#include "Octree/Octree.h"
static constexpr double DENSITY = 9510.77; //una densidad de 1000 kg/m^3 en el sistema de unidades del simulador
static constexpr double MI = 0.5; //Medida de cómo la energía y el momentum del proyectil se acopla al objetivo
//el valor está entre 1/3 y 2/3. 1/3 es para momentum puro y 2/3 para energía pura. En el paper de donde
//obtuve este número se representa como el símbolo "mi" griego pero un sombrero encima


void collisions_for_bodies(Octree *const &octree, NodeOctree *const &node,
    const std::vector<CelestialBody *> &bodies, int begin, int end);
double overlap_body(const Vec3 &center1, const Vec3 &center2, const double radius1, const double radius2) ;
bool overlap_node(const Vec3 &nodeCenter, double nodeSize, const Vec3 &bodyCenter, double bodyRadius) ;
Vec3 closest_point(const Vec3 &nodeCenter, double nodeSize, const Vec3 &bodyCenter) ;
void resolve_collision(CelestialBody *&, CelestialBody *&);

//regímenes:

void merge_regime(CelestialBody *largestBody, CelestialBody *smallestBody);
void super_catastrophic_disruption_regime(CelestialBody *largestBody, CelestialBody *smallestBody);
void erosion_disruption_regime_non_grazing(CelestialBody *largestBody, CelestialBody *smallestBody);
void partial_accretion_regime(CelestialBody *largestBody, CelestialBody *smallestBody);
void erosion_disruption_regime_grazing(CelestialBody *largestBody, CelestialBody *smallestBody);
void hit_and_run_regime(CelestialBody *largestBody, CelestialBody *smallestBody);


//Cálculos para distinguir regímenes:

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

#endif //SIMULADORGRAVITACIONAL_COLLISIONS_H

