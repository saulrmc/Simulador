//
// Created by Saúl on 15/02/2026.
//

#include "Collisions.h"

#include <iostream>

void collisions_for_bodies(Octree *const &octree,
                           std::vector<CelestialBody *> &bodies, int begin, int end) {
    if (begin > end) return;
    if (begin == end) {
        if (bodies.size()<100)
        octree->query_region(overlap,
            resolve_collision, bodies[begin], bodies);

        else octree->query_region(overlap,
            simplified_resolve_collision, bodies[begin], bodies);
        return;
    }
    int middle = begin + (end - begin) / 2;
    collisions_for_bodies(octree, bodies, begin, middle);
    collisions_for_bodies(octree, bodies, middle + 1,  end);
}

void collisions_for_range(Octree *const &octree,
    std::vector<CelestialBody *> &bodies, int begin, int end) {
    for (int i=begin; i<end; i++) {
        octree->query_region(overlap,
                             resolve_collision, bodies[i], bodies);
    }
}
void collisions_for_bodies(Octree *const &octree, std::vector<CelestialBody *> &bodies) {
    if (bodies.size()<100) {
        for (CelestialBody *&body : bodies) {
            octree->query_region(overlap,
                                 resolve_collision, body, bodies);
        }
    }
    else {
        for (CelestialBody *&body : bodies) {
            octree->query_region(overlap,
                                 simplified_resolve_collision, body, bodies);
        }
    }
}

void collisions(Octree *const &octree, std::vector<CelestialBody *> &bodies) {
    if (bodies.size() < 100) collisions_for_bodies(octree, bodies);
    else {
        uint8_t num_threads = 4;
        uint8_t range = bodies.size()/num_threads;
        std::vector<std::thread> threads;
        for (int i = 0; i < num_threads; ++i) {
            int begin = i * range;
            int end = (i == num_threads - 1) ? bodies.size() : (i + 1) * range;
            threads.emplace_back(collisions_for_range, std::ref(octree), std::ref(bodies), begin, end);
        }
        for (auto &thread : threads) thread.join();
    }

}


bool overlap_body(const Vec3 &center1, const Vec3 &center2, const double radius1, const double radius2) {
    double distance = (center2 - center1).magnitude();
    if (radius1 + radius2 - distance >= 0) std::cout << "colision detectada" <<std::endl;
    return radius1 + radius2 - distance >= 0;
}

bool overlap_node(const Vec3 &nodeCenter, double nodeSize, const Vec3 &bodyCenter, double bodyRadius) {
    //para determinar si el cuerpo atraviesa parcialmente el espacio en otro nodo...
    Vec3 closestPoint = closest_point(nodeCenter, nodeSize, bodyCenter);
    double distance = closestPoint.distance(bodyCenter);
    if (distance <= bodyRadius)std::cout << "colision detectada" <<std::endl;
    if (distance <= bodyRadius) return true;
    return false;
}

bool overlap(NodeOctree *const &node,  CelestialBody *const& nodeBody,  CelestialBody*const& body) {
    if (!overlap_node(node->get_node_center(), node->get_node_size(),
                      body->get_position(), body->get_radius()))
        return false;
    if (nodeBody and body)
        return overlap_body(nodeBody->get_position(), body->get_position(),
                            nodeBody->get_radius(), body->get_radius());
    return true;
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
    else closestPoint = closestPoint + Vec3(0, 0, bodyCenter.get_z());


    return closestPoint;
}

void simplified_resolve_collision(CelestialBody *&body1, CelestialBody *&body2, std::vector<CelestialBody *> &bodies) {
    bool overlap = overlap_body(body1->get_position(), body2->get_position(),
        body1->get_radius(), body2->get_radius());
    if (!overlap) return;
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


    //básicamente un merge o hit-and-run
    if (relativeVelocity < mutualEscapeVelMod) merge_regime(largestBody, smallestBody, bodies);
    else {
        double collisionAngle = collision_angle(largestBody->get_velocity(),
        smallestBody->get_velocity(), largestBody->get_position(),
        smallestBody->get_position() );
        double bParameter = sin(collisionAngle);
        double bCrit = largestBody->get_radius()/(largestBody->get_radius() + smallestBody->get_radius());
        bool grazingImpact = true;
        if (bParameter < bCrit) grazingImpact = false;
        if (grazingImpact)
        hit_and_run_regime(largestBody, smallestBody, massInteract, relativeVelocity, bParameter);
        else merge_regime(largestBody, smallestBody, bodies);
    }

}

void resolve_collision(CelestialBody *&body1, CelestialBody *&body2, std::vector<CelestialBody *> &bodies) {
    bool overlap = overlap_body(body1->get_position(), body2->get_position(),
        body1->get_radius(), body2->get_radius());
    if (!overlap) return;

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
        merge_regime(largestBody, smallestBody, bodies);
        return;
    }

    //calcular b_crit, si b < b_crit el impacto es no rozante (no es posible un hit-run)
    double collisionAngle = collision_angle(largestBody->get_velocity(),
        smallestBody->get_velocity(), largestBody->get_position(),
        smallestBody->get_position() );
    double bParameter = sin(collisionAngle);
    double bCrit = largestBody->get_radius()/(largestBody->get_radius() + smallestBody->get_radius());
    bool grazingImpact = true;

    if (bParameter < bCrit) grazingImpact = false;

    //calcular el criterio de disrupción catastrófica y la velocidad crítica de impacto
    const double combinedRadius = pow(largestBody->get_radius() *
        largestBody->get_radius() * largestBody->get_radius() +
        smallestBody->get_radius() * smallestBody->get_radius() * smallestBody->get_radius(), 1.0/3);
    double criticalImpVelMod = critical_impact_velocity_mod(combinedRadius);
    double disruptionCurve = disruption_curve(combinedRadius);
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
    double specificImpEnergySC = specific_impact_energy(0.1 * totalMass,
    disruptionEnergy, largestBody->get_mass(), smallestBody->get_mass());
    double vSupercat = impact_velocity(reducedMass, specificImpEnergySC, totalMass);
    double actualImpactEnergy = specific_impact_energy(reducedMass,
        relativeVelocity, totalMass);

    if (relativeVelocity > vSupercat) super_catastrophic_disruption_regime(largestBody,
        smallestBody, actualImpactEnergy, disruptionEnergy, bParameter);
    else if (relativeVelocity > vErosion) disruption_regime(largestBody, smallestBody,
        actualImpactEnergy, disruptionEnergy, bParameter);
    else {
        if (!grazingImpact) disruption_regime(largestBody, smallestBody,
            actualImpactEnergy, disruptionEnergy, bParameter);
        else hit_and_run_regime(largestBody, smallestBody, massInteract, relativeVelocity,bParameter);
    }
}

//regímenes:

void merge_regime(CelestialBody *&largestBody, CelestialBody *&smallestBody,
    std::vector<CelestialBody *> &bodies) {
    //se debe conservar el momento lineal
    Vec3 initialMomentum = momentum(largestBody->get_velocity(), largestBody->get_mass(),
        smallestBody->get_velocity(), smallestBody->get_mass());
    double density = density_by_mass_and_radius(largestBody->get_mass(),
        largestBody->get_radius());
    largestBody->set_mass(largestBody->get_mass() + smallestBody->get_mass());//absorbe al otro cuerpo
    largestBody->set_velocity(initialMomentum/largestBody->get_mass());//conservación el momentum
    double newRadius = radius_by_density_and_mass(largestBody->get_mass(), density);//se ajusta el nuevo radio
    largestBody->set_radius(newRadius);
    delete_body(bodies, smallestBody->get_index());
}

void super_catastrophic_disruption_regime(CelestialBody *largestBody,
    CelestialBody *smallestBody, double specificImpEnergySC,
    double disruptionEnergy, double bParameter) {
    double mLR = mass_largest_remnant_supcat(specificImpEnergySC, disruptionEnergy,
        largestBody->get_mass() + smallestBody->get_mass());
    compute_remnant_properties_and_velocities(largestBody, smallestBody, bParameter,
        mLR, 1, 2);
}


void disruption_regime(CelestialBody *largestBody, CelestialBody *smallestBody,
    double specificImpEnergyErosion, double disruptionEnergy, double bParameter) {
    //El régimen de disrupción incluye:
    //- El régimen de acreción parcial en el cual la masa del mayor remanente (mLR)
    //es mayor a la masa del cuerpo más grande
    //- El régimen de disrupción catastrófica en el cual la masa del remanente más grande es
    //la mitad de la masa del cuerpo más grande
    //- El régimen de erosión en el cual la masa del remanente más grande es
    //menor que la masa del cuerpo más grande
    double mLR = mass_largest_remnant(specificImpEnergyErosion,
        disruptionEnergy, largestBody->get_mass() + smallestBody->get_mass());
    compute_remnant_properties_and_velocities(largestBody, smallestBody, bParameter,
        mLR, 1, 2);
}

void hit_and_run_regime(CelestialBody *&largestBody, CelestialBody *&smallestBody, double massInteract,
    double impactVelocity, double bParameter) {
    //a pesar de que los nombres son similares a las variables que se calcularon antes en realidad
    //este régimen tiene sus propias variables
    double density = density_by_mass_and_radius(smallestBody->get_mass(), smallestBody->get_radius());
    double combRadius = radius_by_density_and_mass(density,
        smallestBody->get_mass() + massInteract);
    double disruptionCurve = disruption_curve(combRadius);
    double critImpVelMod = critical_impact_velocity_mod(combRadius);
    double reducedMass = smallestBody->get_mass()*massInteract/
        (smallestBody->get_mass() + massInteract);
    double relationMass = massInteract / smallestBody->get_mass();
    double disruptionCriterion = disruption_criterion(disruptionCurve, relationMass);
    double critImpVel = critical_impact_velocity(critImpVelMod, relationMass);
    double specificImpEnergy = specific_impact_energy(reducedMass, impactVelocity,
        smallestBody->get_mass() + massInteract);
    double massLargestRemnant = mass_largest_remnant(specificImpEnergy,
        disruptionCriterion, smallestBody->get_mass() + massInteract);
    if (relationMass < 1.1 and relationMass > 0.9) {
        //usar N1 = 2 y N2 = 4
        compute_remnant_properties_and_velocities(largestBody, smallestBody,
            bParameter, massLargestRemnant, 2, 4);
    }
    else compute_remnant_properties_and_velocities(largestBody, smallestBody,
            bParameter, massLargestRemnant, 1, 2);

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

double mutual_escape_velocity(double mass1, double mass2, const Vec3 &pos1, const Vec3 &pos2) {
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
    cos_angle = (cos_angle < 0) ? -cos_angle : cos_angle;
    return acos(cos_angle);
}

double mass_interact(const Vec3 &vel1, const Vec3 &vel2, const Vec3& center1, const Vec3& center2, double radius2) {
    double collisionAngle = collision_angle(vel1, vel2, center1, center2);
    double l_parameter = (center1 - center2).magnitude() * (1 - sin(collisionAngle));
    return DENSITY * std::numbers::pi * l_parameter*l_parameter * (radius2 - l_parameter/3.0);
}

double disruption_curve(double combinedRadius) {
    double c = 1.9; //Representa la diferencia entre la energía de
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
    //así lo llama en el paper, pero no usa un ángulo
   return sqrt(2.0*disruptionEnergy*totalMass/reducedMass);
}

double specific_impact_energy(double massLR, double disruptionEnergy, double largestMass, double smallestMass) {
    return 2 * disruptionEnergy*(1-massLR/(largestMass+smallestMass));
}

double specific_impact_energy(double reducedMass, double impactVelocity, double totalMass) {
    return 0.5*reducedMass*impactVelocity * impactVelocity/totalMass;
}

double impact_velocity(double reducedMass, double specificImpEnergy, double totalMass) {
    return sqrt(2 * specificImpEnergy*totalMass/reducedMass);
}

double mass_largest_remnant(double specificImpEnergyErosion, double disruptionEnergy, double totalMass) {
    return totalMass * (1 - 0.5*specificImpEnergyErosion/disruptionEnergy);
}

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

void refresh_body(double currentMass, double newMass, double currentRadius,
    const Vec3 &newVelocity, CelestialBody *&body) {
    double density = density_by_mass_and_radius(currentMass, currentRadius);
    body->set_mass(newMass);
    double newRadius = radius_by_density_and_mass(newMass, density);
    body->set_radius(newRadius);
    body->set_velocity(newVelocity);
}

void update_bodies_after_fragmentation(CelestialBody *&largestBody, CelestialBody *&smallestBody,
    double mLR, double mSLR, const Vec3 &newVelLR, const Vec3 &initialMomentum) {
    //el cuerpo más grande se actualiza:
    refresh_body(largestBody->get_mass(),
        mLR, largestBody->get_radius(), newVelLR, largestBody);
    //el cuerpo más pequeño se actualiza
    refresh_body(smallestBody->get_mass(), mSLR,
        smallestBody->get_radius(), newVelLR, smallestBody);
    //se debe corregir la velocidad del segundo fragmento más grande (SLR):
    Vec3 newVelSLR = (initialMomentum - largestBody->get_velocity()*largestBody->get_mass())/
        smallestBody->get_mass();
    smallestBody->set_velocity(newVelSLR);
}

double mass_largest_remnant_supcat(double specificImpEnergySC, double disruptionEnergy,
    double totalMass) {
    const double n =-1.5; //valor aproximado que se sugiere
    const double firstTerm = 0.1/pow(1.8, n);
    return firstTerm * totalMass * pow(specificImpEnergySC/disruptionEnergy, n);
}

void compute_remnant_properties_and_velocities(CelestialBody *&largestBody, CelestialBody *&smallestBody,
    double bParameter, double mLR, int N1, int N2) {
    Vec3 initialMomentum = momentum(largestBody->get_velocity(), largestBody->get_mass(),
            smallestBody->get_velocity(), smallestBody->get_mass());
    double mSLR = mass_second_largest_remnant(mLR,
        largestBody->get_mass() + smallestBody->get_mass(), N1, N2);
    Vec3 velCM = (largestBody->get_velocity() * largestBody->get_mass() +
            smallestBody->get_velocity() * smallestBody->get_mass()) /
                (largestBody->get_mass() + smallestBody->get_mass());
    if (bParameter == 0)//frontal
        update_bodies_after_fragmentation(largestBody, smallestBody,
            mLR, mSLR, velCM, initialMomentum);
    else if (bParameter > 0.7)
        update_bodies_after_fragmentation(largestBody, smallestBody,
            mLR, mSLR, largestBody->get_velocity(), initialMomentum);
    else {
        Vec3 newVelLR;
        if (bParameter < 0.175) newVelLR = velCM * 0.8+ largestBody->get_velocity() * 0.2;
        else if (bParameter < 0.35) newVelLR = velCM * 0.6+ largestBody->get_velocity() * 0.4;
        else if (bParameter < 0.525) newVelLR = velCM * 0.4+ largestBody->get_velocity() * 0.6;
        else newVelLR = velCM * 0.2 + largestBody->get_velocity() * 0.8;
        update_bodies_after_fragmentation(largestBody, smallestBody,
            mLR, mSLR, newVelLR, initialMomentum);
    }
}