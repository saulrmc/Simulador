//
// Created by Saúl on 15/02/2026.
//

#include "Collisions.h"
#include "PhysicsUtils.h"

#include <algorithm>
#include <iostream>

/*  Modelo de colisiones obtenido de:
 *  Leinhardt, Z. M., & Stewart, S. T. (2012).
 * "Collisions between gravity-dominated bodies. I. Outcome regimes and scaling laws".
 * The Astrophysical Journal, 745(1), 79.
 */

//Algoritmo adaptado de: A.1. A General Formulation for Collision Outcomes de Leinhardt & Stewart (2012)
void simplified_resolve_collision(CelestialBodies &bodies, int body1Index, int body2Index) {
    if (body1Index < 0 || body1Index >= (int)bodies.size()) return;
    if (body2Index < 0 || body2Index >= (int)bodies.size()) return;
    if (bodies.is_deleted(body1Index) || bodies.is_deleted(body2Index)) return;
    if (bodies.get_mass(body1Index) <= 0 || bodies.get_mass(body2Index) <= 0) return;
    if (bodies.get_radius(body1Index) <= 0 || bodies.get_radius(body2Index) <= 0) return;
    bool overlap = overlap_body(
        bodies.get_position(body1Index), bodies.get_position(body2Index),
        bodies.get_radius(body1Index), bodies.get_radius(body2Index));
    if (!overlap) return;
    Vec3 normal = (bodies.get_position(body2Index) - bodies.get_position(body1Index)).normalize();
    Vec3 relVel = bodies.get_velocity(body2Index) - bodies.get_velocity(body1Index);
    if (relVel.dot(normal) >= 0) {
        return;
    }

    int largestBody = (bodies.get_mass(body1Index) >= bodies.get_mass(body2Index)) ? body1Index : body2Index;
    int smallestBody = (bodies.get_mass(body1Index) >= bodies.get_mass(body2Index)) ? body2Index : body1Index;

    if (bodies.get_radius(largestBody) + bodies.get_radius(smallestBody)
        - (bodies.get_position(largestBody) - bodies.get_position(smallestBody)).magnitude() >= 0) {

        const double relativeVelocity =
            (bodies.get_velocity(largestBody) - bodies.get_velocity(smallestBody)).magnitude();
        double massInteract = mass_interact(bodies, largestBody, smallestBody);
        double density1 = density_by_mass_and_radius(bodies.get_mass(largestBody), bodies.get_radius(largestBody));
        double density2 = density_by_mass_and_radius(bodies.get_mass(smallestBody), bodies.get_radius(smallestBody));
        double avgDensity = (density1 + density2)/2;
        const double mutualEscapeVelMod = mutual_escape_velocity_mod(
            bodies.get_mass(largestBody), massInteract, avgDensity);

        if (relativeVelocity < mutualEscapeVelMod) merge_regime(bodies, largestBody, smallestBody);
        else {
            double collisionAngle = collision_angle(
                bodies.get_velocity(largestBody), bodies.get_velocity(smallestBody),
                bodies.get_position(largestBody), bodies.get_position(smallestBody));
            double bParameter = sin(collisionAngle);
            double bCrit = bodies.get_radius(largestBody)/
                (bodies.get_radius(largestBody) + bodies.get_radius(smallestBody));
            bool grazingImpact = (bParameter >= bCrit);
            if (grazingImpact)
                hit_and_run_regime(bodies, largestBody, smallestBody, massInteract);
            else merge_regime(bodies, largestBody, smallestBody);
        }
    }
}

//Algoritmo adaptado de: A.1. A General Formulation for Collision Outcomes de Leinhardt & Stewart (2012)
void resolve_collision(CelestialBodies &bodies, int body1Index, int body2Index) {
    if (body1Index < 0 || body1Index >= (int)bodies.size()) return;
    if (body2Index < 0 || body2Index >= (int)bodies.size()) return;
    if (bodies.is_deleted(body1Index) || bodies.is_deleted(body2Index)) return;
    if (bodies.get_mass(body1Index) <= 0 || bodies.get_mass(body2Index) <= 0) return;
    if (bodies.get_radius(body1Index) <= 0 || bodies.get_radius(body2Index) <= 0) return;
    bool overlap = overlap_body(
        bodies.get_position(body1Index), bodies.get_position(body2Index),
        bodies.get_radius(body1Index), bodies.get_radius(body2Index));
    if (!overlap) return;
    Vec3 normal = (bodies.get_position(body2Index) - bodies.get_position(body1Index)).normalize();
    Vec3 relVel = bodies.get_velocity(body2Index) - bodies.get_velocity(body1Index);
    if (relVel.dot(normal) >= 0) {
        return;
    }

    int largestBody = (bodies.get_mass(body1Index) >= bodies.get_mass(body2Index)) ? body1Index : body2Index;
    int smallestBody = (bodies.get_mass(body1Index) >= bodies.get_mass(body2Index)) ? body2Index : body1Index;

    double body1Mass = bodies.get_mass(largestBody);
    double body1Radius = bodies.get_radius(largestBody);
    Vec3 body1Position = bodies.get_position(largestBody);
    Vec3 body1Velocity = bodies.get_velocity(largestBody);

    double body2Mass = bodies.get_mass(smallestBody);
    double body2Radius = bodies.get_radius(smallestBody);
    Vec3 body2Position = bodies.get_position(smallestBody);
    Vec3 body2Velocity = bodies.get_velocity(smallestBody);

    if (body1Radius + body2Radius
        - (body1Position - body2Position).magnitude() >= 0)
        std::cout << "colision detectada" <<std::endl;
    double totalMass = body2Mass + body1Mass;
    const double relativeVelocity = (body1Velocity - body2Velocity).magnitude();
    double massInteract = mass_interact(bodies, largestBody, smallestBody);
    double density1 = density_by_mass_and_radius(body1Mass, body1Radius);
    double density2 = density_by_mass_and_radius(body2Mass, body2Radius);
    double avgDensity = (density1 + density2)/2;
    const double mutualEscapeVelMod = mutual_escape_velocity_mod(
        body1Mass, massInteract, avgDensity);

    if (relativeVelocity < mutualEscapeVelMod) {
        merge_regime(bodies, largestBody, smallestBody);
        return;
    }

    double collisionAngle = collision_angle(body1Velocity,
        body2Velocity, body1Position, body2Position);
    double bParameter = sin(collisionAngle);
    double bCrit = body1Radius/(body1Radius + body2Radius);
    bool grazingImpact = (bParameter >= bCrit);

    const double combinedRadius =
        pow(body1Radius *body1Radius * body1Radius + body2Radius * body2Radius * body2Radius, 1.0/3);
    double criticalImpVelMod = critical_impact_velocity_mod(combinedRadius, avgDensity);
    double disruptionCurve = disruption_curve(combinedRadius, avgDensity);
    double reducedMass = body2Mass * body1Mass / totalMass;
    double reducedMassMod = massInteract * body1Mass/
                            (massInteract + body1Mass);
    double disruptionCriterion = disruption_criterion(disruptionCurve,
        body2Mass/body1Mass);
    double criticalImpVel = critical_impact_velocity(criticalImpVelMod,
        body2Mass/body1Mass);
    double disruptionEnergy = disruption_energy_by_angle(disruptionCriterion,
        reducedMass, reducedMassMod);
    double vErosion = impact_velocity(reducedMass,
        specific_impact_energy(body1Mass, disruptionEnergy, body1Mass, body2Mass),
        totalMass);
    double specificImpEnergySC = specific_impact_energy(0.1 * totalMass,
        disruptionEnergy, body1Mass, body2Mass);
    double vSupercat = impact_velocity(reducedMass, specificImpEnergySC, totalMass);

    if (relativeVelocity > vSupercat) super_catastrophic_disruption_regime(bodies,
        largestBody, smallestBody, disruptionEnergy);
    else if (relativeVelocity > vErosion) disruption_regime(bodies,
        largestBody, smallestBody, disruptionEnergy);
    else {
        if (!grazingImpact) disruption_regime(bodies,
            largestBody, smallestBody, disruptionEnergy);
        else hit_and_run_regime(bodies,
            largestBody, smallestBody, massInteract);
    }
}

//Régimen de fusión obtenido de: Leinhardt & Stewart (2012)
void merge_regime(CelestialBodies &bodies,
    int largestBodyIndex, int smallestBodyIndex) {
    double massL = bodies.get_mass(largestBodyIndex);
    double massS = bodies.get_mass(smallestBodyIndex);
    double radiusL = bodies.get_radius(largestBodyIndex);
    Vec3 velL = bodies.get_velocity(largestBodyIndex);
    Vec3 velS = bodies.get_velocity(smallestBodyIndex);

    Vec3 initialMomentum = momentum(velL, massL, velS, massS);
    double density = density_by_mass_and_radius(massL, radiusL);
    bodies.set_mass(massL + massS, largestBodyIndex);
    bodies.set_velocity(initialMomentum/(massL + massS), largestBodyIndex);
    double newRadius = radius_by_density_and_mass(massL + massS, density);
    bodies.set_radius(newRadius, largestBodyIndex);
    bodies.mark_deleted(smallestBodyIndex);
}

//Régimen super catastrófico obtenido de: Leinhardt & Stewart (2012)
void super_catastrophic_disruption_regime(CelestialBodies &bodies,
    int largestBodyIndex, int smallestBodyIndex, double disruptionEnergy) {
    double massL = bodies.get_mass(largestBodyIndex);
    double massS = bodies.get_mass(smallestBodyIndex);
    double totalMass = massL + massS;
    double reducedMass = massL * massS / totalMass;
    double relativeVelocity =
        (bodies.get_velocity(largestBodyIndex) - bodies.get_velocity(smallestBodyIndex)).magnitude();
    double actualImpactEnergy = specific_impact_energy(reducedMass, relativeVelocity, totalMass);
    double mLR = mass_largest_remnant_supcat(actualImpactEnergy, disruptionEnergy, totalMass);
    compute_remnant_properties_and_velocities(bodies,
        largestBodyIndex, smallestBodyIndex, mLR, 1, 2);
}

//Régimen de disrupción obtenido de: Leinhardt & Stewart (2012)
void disruption_regime(CelestialBodies &bodies,
    int largestBodyIndex, int smallestBodyIndex, double disruptionEnergy) {
    double massL = bodies.get_mass(largestBodyIndex);
    double massS = bodies.get_mass(smallestBodyIndex);
    double totalMass = massL + massS;
    double specificImpEnergyErosion = specific_impact_energy(
        massL, disruptionEnergy, massL, massS);
    double mLR = mass_largest_remnant(specificImpEnergyErosion,
        disruptionEnergy, totalMass);
    compute_remnant_properties_and_velocities(bodies,
        largestBodyIndex, smallestBodyIndex, mLR, 1, 2);
}

//Régimen hit-and-run obtenido de: Leinhardt & Stewart (2012)
void hit_and_run_regime(CelestialBodies &bodies,
    int largestBodyIndex, int smallestBodyIndex, double massInteract) {
    double massL = bodies.get_mass(largestBodyIndex);
    double massS = bodies.get_mass(smallestBodyIndex);
    double radiusL = bodies.get_radius(largestBodyIndex);
    double radiusS = bodies.get_radius(smallestBodyIndex);
    Vec3 velL = bodies.get_velocity(largestBodyIndex);
    Vec3 velS = bodies.get_velocity(smallestBodyIndex);

    const double impactVelocity = (velL - velS).magnitude();
    double density = density_by_mass_and_radius(massS, radiusS);
    double combRadius = radius_by_density_and_mass(massS + massInteract, density);
    double densityL = density_by_mass_and_radius(massL, radiusL);
    double densityS = density_by_mass_and_radius(massS, radiusS);
    double avgDensity = (densityL + densityS)/2;
    // double bParameter = sin(collision_angle(velL, velS,
    //     bodies.get_position(largestBodyIndex), bodies.get_position(smallestBodyIndex)));
    double disruptionCurve = disruption_curve(combRadius, avgDensity);
    double critImpVelMod = critical_impact_velocity_mod(combRadius, avgDensity);
    double reducedMass = massS * massInteract/
        (massS + massInteract);
    double relationMass = massInteract / massS;
    double disruptionCriterion = disruption_criterion(disruptionCurve, relationMass);
    double critImpVel = critical_impact_velocity(critImpVelMod, relationMass);
    double specificImpEnergy = specific_impact_energy(reducedMass, impactVelocity,
        massS + massInteract);
    double massLargestRemnant = mass_largest_remnant(specificImpEnergy,
        disruptionCriterion, massS + massInteract);
    if (relationMass < 1.1 and relationMass > 0.9) {
        compute_remnant_properties_and_velocities(bodies,
            largestBodyIndex, smallestBodyIndex, massLargestRemnant, 2, 4);
    }
    else compute_remnant_properties_and_velocities(bodies,
        largestBodyIndex, smallestBodyIndex, massLargestRemnant, 1, 2);
}


//Cálculos para distinguir regímenes:

double mutual_escape_velocity(double mass1, double mass2, const Vec3 &pos1, const Vec3 &pos2) {
    double distance = (pos2 - pos1).magnitude();
    return sqrt(2.0*units::G*(mass1 + mass2)/distance);
}

//Adaptado de: Leinhardt & Stewart (2012). Ecuación (53)
double mutual_escape_velocity_mod(double largestMass, double massInteract, double avgDensity) {
    const double R = pow(3*(largestMass + massInteract)/(4 * std::numbers::pi * avgDensity), 1.0/3);
    return sqrt(2*units::G*(largestMass + massInteract)/R);
}


double catastrophic_disruption_criterion(const Vec3 &vel1, const Vec3 &vel2, double combinedRadius,
    double avgDensity) {
    const Vec3 relativeVelocity = vel1-vel2;
    const double q_g = 1;
    const double density = avgDensity;
    const double mi = MI;

    return q_g * pow(density * units::G, 3.0*mi/2) * pow(combinedRadius, 3*mi)
    * pow(relativeVelocity.magnitude(), 2 - 3*mi);
}

double collision_timescale(double radius1, double radius2, double distance, double relativeVelocity) {
    return (radius1 + radius2 - distance) / std::max(relativeVelocity, 1e-8);
}

//Adaptado de: Leinhardt & Stewart (2012). Ecuación (7) y Figura (2)
double collision_angle(const Vec3 &vel1, const Vec3 &vel2, const Vec3& center1, const Vec3& center2) {
    const Vec3 relativeVelocity = vel1 - vel2;
    const Vec3 distance = center1-center2;
    double denom = relativeVelocity.magnitude()*distance.magnitude();
    if (denom < 1e-15) return 0.0;
    double cos_angle = relativeVelocity.dot(distance)/denom;
    cos_angle = (cos_angle <= -1)? -1: cos_angle;
    cos_angle = (cos_angle >= 1)? 1: cos_angle;
    double angle = acos(cos_angle);
    return angle;
}

//Adaptado de: Leinhardt & Stewart (2012). Ecuación (10)
double mass_interact(CelestialBodies &bodies, int largestBodyIndex, int smallestBodyIndex) {
    double massL = bodies.get_mass(largestBodyIndex);
    double radiusL = bodies.get_radius(largestBodyIndex);
    Vec3 posL = bodies.get_position(largestBodyIndex);
    Vec3 velL = bodies.get_velocity(largestBodyIndex);
    double massS = bodies.get_mass(smallestBodyIndex);
    double radiusS = bodies.get_radius(smallestBodyIndex);
    Vec3 posS = bodies.get_position(smallestBodyIndex);
    Vec3 velS = bodies.get_velocity(smallestBodyIndex);

    double collisionAngle = collision_angle(velL, velS, posL, posS);
    double l_parameter = (posL - posS).magnitude() * (1 - sin(collisionAngle));
    double density1 = density_by_mass_and_radius(massL, radiusL);
    double density2 = density_by_mass_and_radius(massS, radiusS);
    double avgDensity = (density1 + density2)/2;
    return avgDensity * std::numbers::pi * l_parameter*l_parameter *
        (radiusS - l_parameter/3.0);
}

//Obtenido de: Leinhardt & Stewart (2012). Ecuación (28)
double disruption_curve(double combinedRadius, double avgDensity) {
    double c = 1.9;
    return c * (4.0/5) * std::numbers::pi * avgDensity * units::G * combinedRadius * combinedRadius;
}

//Obtenido de: Leinhardt & Stewart (2012). Ecuación (30)
double critical_impact_velocity_mod(double combinedRadius, double avgDensity) {
    double c = 1.9;
    return sqrt(32.0 * std::numbers::pi * c/5) * sqrt(avgDensity * units::G) * combinedRadius;
}

//Obtenido de: Leinhardt & Stewart (2012). Ecuación (23)
double disruption_criterion(double disruptionCurve, double relationMass) {
    double exp = 2.0/(3*MI) - 1;
    double secondTerm = pow(0.25*(relationMass + 1)*
        (relationMass + 1)/relationMass, exp);
    return disruptionCurve*secondTerm;
}

//Obtenido de: Leinhardt & Stewart (2012). Ecuación (22)
double critical_impact_velocity(double criticalImpVelMod, double relationMass) {
    double exp = 1.0/(3*MI);
    double firstTerm = pow(0.25*(relationMass + 1) *
        (relationMass + 1)/relationMass,exp);
    return firstTerm * criticalImpVelMod;
}

//Obtenido de: Leinhardt & Stewart (2012). Ecuación (15)
double disruption_energy_by_angle(double disruptionCriterion, double reducedMass, double reducedMassMod) {
    double exp = 2 - 3.0*MI/2;
    double firstTerm = pow(reducedMass/reducedMassMod,exp);
    return firstTerm * disruptionCriterion;
}


//Adaptado de: Leinhardt & Stewart (2012). Ecuación (5)
double specific_impact_energy(double massLR, double disruptionEnergy, double largestMass, double smallestMass) {
    return 2 * disruptionEnergy*(1-massLR/(largestMass+smallestMass));
}

//Obtenido de: Leinhardt & Stewart (2012). Ecuación (1)
double specific_impact_energy(double reducedMass, double impactVelocity, double totalMass) {
    return 0.5*reducedMass*impactVelocity * impactVelocity/totalMass;
}

//Obtenido de: Leinhardt & Stewart (2012). Ecuación (16)
double impact_velocity(double reducedMass, double specificImpEnergy, double totalMass) {
    return sqrt(2 * specificImpEnergy*totalMass/reducedMass);
}

//Adaptado de: Leinhardt & Stewart (2012). Ecuación (5)
double mass_largest_remnant(double specificImpEnergyErosion, double disruptionEnergy, double totalMass) {
    return totalMass * (1 - 0.5*specificImpEnergyErosion/disruptionEnergy);
}

//Obtenido de: Leinhardt & Stewart (2012). Ecuación (37)
double mass_second_largest_remnant(double mLR, double totalMass, int N1, int N2) {
    return (3 - 2.85)*(1 - N1*mLR/totalMass)/(N2 * 2.85);
}

Vec3 momentum(const Vec3 &vel1, double mass1, const Vec3 &vel2, double mass2) {
    return vel1 * mass1 + vel2 * mass2;
}

Vec3 velCM(const Vec3 &vel1, double mass1, const Vec3 &vel2, double mass2) {
    return (vel1 * mass1 + vel2 * mass2)/(mass1 + mass2);
}

double radius_by_density_and_mass(double mass, double density) {
    return pow(3 * mass/(4 * std::numbers::pi * density), 1.0/3);
}

double density_by_mass_and_radius(double mass, double radius) {
    return 3 * mass / (4 * std::numbers::pi * pow(radius, 3));
}

void refresh_body(CelestialBodies &bodies, int indexBody, double newMass,
    const Vec3 &newVelocity) {
    double currentMass = bodies.get_mass(indexBody);
    double currentRadius = bodies.get_radius(indexBody);
    double density = density_by_mass_and_radius(currentMass, currentRadius);
    bodies.set_mass(newMass, indexBody);
    double newRadius = radius_by_density_and_mass(newMass, density);
    bodies.set_radius(newRadius, indexBody);
    bodies.set_velocity(newVelocity, indexBody);
}

//para separar los cuerpos después de la colision
void fix_positions_after_fragmentation(CelestialBodies &bodies,
    int largestBodyIndex, int smallestBodyIndex) {
    double largestBodyMass = bodies.get_mass(largestBodyIndex);
    double largestBodyRadius = bodies.get_radius(largestBodyIndex);
    Vec3 largestBodyPos = bodies.get_position(largestBodyIndex);
    double smallestBodyMass = bodies.get_mass(smallestBodyIndex);
    double smallestBodyRadius = bodies.get_radius(smallestBodyIndex);
    Vec3 smallestBodyPos = bodies.get_position(smallestBodyIndex);

    double penetration = largestBodyRadius + smallestBodyRadius -
        (largestBodyPos-smallestBodyPos).magnitude();
    if (penetration > 0) {
        Vec3 normal;
        Vec3 delta = smallestBodyPos - largestBodyPos;
        double dist = delta.magnitude();
        if (dist < 1e-8) normal = Vec3(1,0,0);
        else normal = delta/dist;
        double totalMass = largestBodyMass + smallestBodyMass;

        double w1 = smallestBodyMass / totalMass;
        double w2 = largestBodyMass / totalMass;

        bodies.set_position(largestBodyPos - normal * penetration * w1, largestBodyIndex);
        bodies.set_position(smallestBodyPos + normal * penetration * w2, smallestBodyIndex);
    }
}

//OBtenido de: Leinhardt & Stewart (2012). Ecuación (44)
double mass_largest_remnant_supcat(double specificImpEnergySC, double disruptionEnergy,
    double totalMass) {
    const double n =-1.5;
    const double firstTerm = 0.1/pow(1.8, n);
    return firstTerm * totalMass * pow(specificImpEnergySC/disruptionEnergy, n);
}

void compute_remnant_properties_and_velocities(CelestialBodies &bodies,
    int largestBodyIndex, int smallestBodyIndex, double mLR, int N1, int N2) {
    double massL = bodies.get_mass(largestBodyIndex);
    double massS = bodies.get_mass(smallestBodyIndex);
    double totalMass = massL + massS;
    Vec3 velL = bodies.get_velocity(largestBodyIndex);
    Vec3 velS = bodies.get_velocity(smallestBodyIndex);

    mLR = std::clamp(mLR, 0.0, totalMass);
    double mSLR = mass_second_largest_remnant(mLR, totalMass, N1, N2);
    mSLR = std::clamp(mSLR, 0.0, totalMass - mLR);

    if (totalMass - mLR < 1e-15 || mSLR < 1e-15) {
        merge_regime(bodies, largestBodyIndex, smallestBodyIndex);
        return;
    }

    Vec3 initialMomentum = momentum(velL, massL, velS, massS);
    Vec3 vCM = (velL * massL + velS * massS) / totalMass;

    double collisionAngle = collision_angle(velL, velS,
        bodies.get_position(largestBodyIndex), bodies.get_position(smallestBodyIndex));
    double bParameter = sin(collisionAngle);

    Vec3 newVelLR;
    if (bParameter == 0)
        newVelLR = vCM;
    else if (bParameter > 0.7)
        newVelLR = velL;
    else {
        if (bParameter < 0.175) newVelLR = vCM * 0.8+ velL * 0.2;
        else if (bParameter < 0.35) newVelLR = vCM * 0.6+ velL * 0.4;
        else if (bParameter < 0.525) newVelLR = vCM * 0.4+ velL * 0.6;
        else newVelLR = vCM * 0.2 + velL * 0.8;
    }

    refresh_body(bodies, largestBodyIndex, mLR, newVelLR);
    refresh_body(bodies, smallestBodyIndex, mSLR, newVelLR);
    Vec3 newVelSLR = (initialMomentum - newVelLR * mLR) / mSLR;
    bodies.set_velocity(newVelSLR, smallestBodyIndex);
    fix_positions_after_fragmentation(bodies, largestBodyIndex, smallestBodyIndex);
}
