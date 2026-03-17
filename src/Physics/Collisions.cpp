//
// Created by Saúl on 15/02/2026.
//

#include "Collisions.h"

#include <algorithm>

void collisions_for_bodies(Octree *const &octree, NodeOctree *const &node,
                           const std::vector<CelestialBody *> &bodies, int begin, int end) {
    if (begin > end) return;
    if (begin == end) {
        octree->query_region(node, overlap_node, resolve_collision, bodies[begin]);
    }
    int middle = begin + (end - begin) / 2;
    collisions_for_bodies(octree, node, bodies, begin, middle);
    collisions_for_bodies(octree, node, bodies, middle + 1,  end);
}

double overlap_body(const Vec3 &center1, const Vec3 &center2, const double radius1, const double radius2) {
    double distance = (center2 - center1).magnitude();
    return radius1 + radius2 - distance;
}

bool overlap_node(const Vec3 &nodeCenter, double nodeSize, const Vec3 &bodyCenter, double bodyRadius) {
    //para determinar si el cuerpo atraviesa parcialmente el espacio en otro nodo...
    Vec3 closestPoint = closest_point(nodeCenter, nodeSize, bodyCenter);
    double distance = closestPoint.distance(bodyCenter);
    if (distance <= bodyRadius) return true;
    return false;
}

//el punto más cercano de un nodo a un cuerpo
Vec3 closest_point(const Vec3 &nodeCenter, double nodeSize, const Vec3 &bodyCenter) {
    Vec3 closestPoint = nodeCenter;
    if (nodeCenter.get_x() + nodeSize/2 < bodyCenter.get_x()) {
        closestPoint = closestPoint + Vec3(nodeSize/2, 0, 0);
    }
    else if (nodeCenter.get_x() - nodeSize/2 > bodyCenter.get_x()) {
        closestPoint = closestPoint - Vec3(nodeSize/2, 0, 0);
    }
    else closestPoint = closestPoint + Vec3(bodyCenter.get_x(), 0, 0);


    if (nodeCenter.get_y() + nodeSize/2 < bodyCenter.get_y()) {
        closestPoint = closestPoint + Vec3(0, nodeSize/2, 0);
    }
    else if (nodeCenter.get_y() - nodeSize/2 > bodyCenter.get_y()) {
        closestPoint = closestPoint - Vec3(0, nodeSize/2, 0);
    }
    else closestPoint = closestPoint + Vec3(0, bodyCenter.get_y(), 0);


    if (nodeCenter.get_z() + nodeSize/2 < bodyCenter.get_z()) {
        closestPoint = closestPoint + Vec3(0, 0, nodeSize/2);
    }
    else if (nodeCenter.get_z() - nodeSize/2 > bodyCenter.get_z()) {
        closestPoint = closestPoint - Vec3(0, 0, nodeSize/2);
    }
    else closestPoint = closestPoint + Vec3(0, 0, bodyCenter.get_x());


    return closestPoint;
}

void resolve_collision(CelestialBody *&body1, CelestialBody *&body2) {
    if (body2->get_id() > body1->get_id()) return; //para solo resolver las colisiones de un lado del par
    double overlap = overlap_body(body1->get_position(), body2->get_position(),
        body1->get_radius(), body2->get_radius());
    if (overlap <= 0) return;

    CelestialBody *largestBody = (body1->get_mass() >= body2->get_mass()) ? body1 : body2;
    CelestialBody *smallestBody = (body1->get_mass() > body2->get_mass()) ? body2 : body1;
    double totalMass = smallestBody->get_mass() + largestBody->get_mass();
    const double relativeVelocity = (largestBody->get_velocity()
        -smallestBody->get_velocity()).magnitude();
    double massInteract = mass_interact(largestBody->get_velocity(),
        smallestBody->get_velocity(), largestBody->get_position(),
        smallestBody->get_position(), smallestBody->get_mass());
    const double mutualEscapeVelMod = mutual_escape_velocity_mod(
        largestBody->get_mass(), massInteract);

    if (relativeVelocity < mutualEscapeVelMod) {
        merge_regime(largestBody, smallestBody);
        return;
    }

    //calcular bcrit, si b < bcrit el impacto es no rozante (no es posible un hit-run)
    double collisionAngle = collision_angle(largestBody->get_velocity(),
        smallestBody->get_velocity(), body1->get_position(),
        body2->get_position() );
    double bParameter = sin(collisionAngle);
    double bCrit = largestBody->get_radius()/(largestBody->get_radius() + smallestBody->get_radius());
    bool grazingImpact = true;

    if (bParameter < bCrit) grazingImpact = false;

    //calcular el criterio de disrupción catastrófica y la velocidad crítica de impacto
    const double combinedRadius = pow(largestBody->get_radius() *
        largestBody->get_radius() * largestBody->get_radius() +
        smallestBody->get_radius() * smallestBody->get_radius() * smallestBody->get_radius(), 1.0/3);
    double criticalImpVelMod = critical_impact_velocity_mod(combinedRadius);
    double disruptionCurve = disruption_curve(largestBody->get_radius(), smallestBody->get_radius());
    double reducedMass = smallestBody->get_mass() * largestBody->get_mass() / totalMass;
    double reducedMassMod = massInteract * largestBody->get_mass()/
                            (massInteract + largestBody->get_mass());
    double disruptionCriterion = disruption_criterion(disruptionCurve,
        smallestBody->get_mass()/largestBody->get_mass());
    double criticalImpVel = critical_impact_velocity(criticalImpVelMod,
        smallestBody->get_mass()/largestBody->get_mass());
    double disruptionEnergy = disruption_energy_by_angle(disruptionCriterion,
        reducedMass, reducedMassMod);
    double criticalImpVelByAngle = critical_impact_velocity_by_angle(disruptionEnergy,
        totalMass, reducedMass);
    double specificImpEnergyErosion = specific_impact_energy(
        largestBody->get_mass(), disruptionEnergy,
        largestBody->get_mass(), smallestBody->get_mass());//Specific impact energy
    //for the collision in center of mass frame

    double vErosion = impact_velocity(reducedMass, specificImpEnergyErosion,
        totalMass);
    double specificImpEnergySC =    specific_impact_energy(0.1 * totalMass,
    disruptionEnergy, largestBody->get_mass(), smallestBody->get_mass());
    double vSupercat = impact_velocity(reducedMass, specificImpEnergySC, totalMass);

    if (relativeVelocity > vSupercat) super_catastrophic_disruption_regime(largestBody, smallestBody);
    else {
        if (!grazingImpact) {
            if (relativeVelocity > vErosion) erosion_disruption_regime_non_grazing(largestBody, smallestBody);
            else partial_accretion_regime(largestBody, smallestBody);
        }
        else {
            if (relativeVelocity > vErosion)
                erosion_disruption_regime_grazing(largestBody, smallestBody);// la ley universal
            //para la masa del remanente más grande se cumple si
            //esa masa es menor que la masa del cuerpo más grande (masivo)

            else hit_and_run_regime(largestBody, smallestBody);
        }
    }
}

//regímenes:

void merge_regime(CelestialBody *largestBody, CelestialBody *smallestBody) {
    //se debe conservar el momento lineal

}

void super_catastrophic_disruption_regime(CelestialBody *largestBody, CelestialBody *smallestBody) {
}

void erosion_disruption_regime_non_grazing(CelestialBody *largestBody, CelestialBody *smallestBody) {
}

void partial_accretion_regime(CelestialBody *largestBody, CelestialBody *smallestBody) {
}

void erosion_disruption_regime_grazing(CelestialBody *largestBody, CelestialBody *smallestBody) {
}

void hit_and_run_regime(CelestialBody *largestBody, CelestialBody *smallestBody) {
}


//Cálculos para distinguir regímenes:

// double specific_impact_energy(const double mass1, const double mass2, const Vec3 &relativeVelocity) {
//     //se obtiene dividiendo la energía total de los dos cuerpos cambiada al sistema del centro de masa por
//     //la masa total de dicho sistema
//     const double reducedMass = mass1*mass2/(mass1+mass2);
//     return 1.0/2*reducedMass*relativeVelocity.dot(relativeVelocity)/(mass1 + mass2);
// }

// double effective_specific_impact_energy(const double mass1, const double mass2, const Vec3 &vel1, const Vec3 &vel2,
//     const Vec3& center1, const Vec3& center2) {
//     //aunque en realidad en cálculo debería ser con las partes de las masas que van a interactuar pero... :/
//     const Vec3 relativeVelocity = vel1-vel2;
//     const Vec3 distance = center1-center2;
//     double energy = specific_impact_energy(mass1, mass2, relativeVelocity);
//     double cos_angle = relativeVelocity.dot(distance)/(relativeVelocity.magnitude()*distance.magnitude());
//     return energy*cos_angle*cos_angle;
// }

double mutual_escape_velocity(double mass1, double mass2, Vec3 pos1, Vec3 pos2) {
    double distance = (pos2 - pos1).magnitude();
    return sqrt(2.0*units::G*(mass1 + mass2)/distance);
}

double mutual_escape_velocity_mod(double largestMass, double massInteract) {
    const double R = pow(3*(largestMass + massInteract)/(4 * std::numbers::pi * DENSITY), 1.0/3);
    return sqrt(2*units::G*(largestMass + massInteract)/R);
}

double catastrophic_disruption_criterion(const Vec3 &vel1, const Vec3 &vel2, double combinedRadius) {
    // se asume que el régimen de fuerza es despreciable para satélites, planetas y demás cuerpos más masivos...

    const Vec3 relativeVelocity = vel1-vel2;
    //POR AHORA se va a asumir que los cuerpos son de densidades iguales
    const double q_g = 1; //coeficiente adimensional con valor cercano a 1
    const double density = DENSITY; //densidad arbitraria entre los dos cuerpos
    const double mi = MI;

    return q_g * pow(density * units::G, 3.0*mi/2) * pow(combinedRadius, 3*mi)
    * pow(relativeVelocity.magnitude(), 2 - 3*mi);
}

double collision_timescale(double radius1, double radius2, double distance, double relativeVelocity) {
    return (radius1 + radius2 - distance) / std::max(relativeVelocity, 1e-8);
}

double collision_angle(const Vec3 &vel1, const Vec3 &vel2, const Vec3& center1, const Vec3& center2) {
    const Vec3 relativeVelocity = vel1 - vel2;
    const Vec3 distance = center1-center2;
    double cos_angle = relativeVelocity.dot(distance)/(relativeVelocity.magnitude()*distance.magnitude());
    return acos(cos_angle);
}

double mass_interact(const Vec3 &vel1, const Vec3 &vel2, const Vec3& center1, const Vec3& center2, double radius2) {
    double collisionAngle = collision_angle(vel1, vel2, center1, center2);
    double l_parameter = (center1 - center2).magnitude() * (1 - sin(collisionAngle));
    return DENSITY * std::numbers::pi * l_parameter*l_parameter * (radius2 - l_parameter/3.0);
}

double disruption_curve(double radius1, double radius2) {
    const double combinedRadius = pow(radius1*radius1*radius1 + radius2*radius2*radius2, 1.0/3);
    double c = 1.9; //representa la diferencia entre la energía de
        //enlace gravitacional y el criterio de disipación para masas iguales. El valor recomendado
        //para cuerpos como planetas es de 1.9+-0.3
    return c * (4.0/5) * std::numbers::pi * DENSITY * units::G * combinedRadius * combinedRadius;
}

double critical_impact_velocity_mod(double combinedRadius) {
    double c = 1.9;
    return sqrt(32.0 * std::numbers::pi * c/5) * sqrt(DENSITY * units::G) * combinedRadius;
}

double disruption_criterion(double disruptionCurve, double relationMass) {
    double exp = 2.0/(3*MI) - 1;
    double secondTerm = pow(0.25*(relationMass + 1)*
        (relationMass + 1)/relationMass, exp);
    return disruptionCurve*secondTerm;
}

double critical_impact_velocity(double criticalImpVelMod, double relationMass) {
    double exp = 1.0/(3*MI);
    double firstTerm = pow(0.25*(relationMass + 1) *
        (relationMass + 1)/relationMass,exp);
    return firstTerm * criticalImpVelMod;
}

double disruption_energy_by_angle(double disruptionCriterion, double reducedMass, double reducedMassMod) {
    double exp = 2 - 3.0*MI/2;
    double firstTerm = pow(reducedMass/reducedMassMod,exp);
    return firstTerm * disruptionCriterion;
}

double critical_impact_velocity_by_angle(double disruptionEnergy, double totalMass, double reducedMass) {
    //así lo llama en el paper pero no usa un ángulo
   return sqrt(2.0*disruptionEnergy*totalMass/reducedMass);
}

double specific_impact_energy(double massLR, double disruptionEnergy, double largestMass, double smallestMass) {
    return 2 * disruptionEnergy*(1-massLR/(largestMass+smallestMass));
}

double impact_velocity(double reducedMass, double specificImpEnergy, double totalMass) {
    return sqrt(2 * specificImpEnergy*totalMass/reducedMass);
}