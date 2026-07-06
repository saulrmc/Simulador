//
// Created by Saúl on 15/02/2026.
//

#include "Collisions.h"

#include <iostream>

// void collisions_for_range(Octree*const &octree,
//     std::vector<CelestialBodies *> &bodies, int begin, int end) {
//     for (int i=begin; i<end; i++) {
//         octree->query_region(overlap_node,
//                              resolve_collision, bodies[i], bodies);
//     }
// }
void collisions(Octree *const &octree, CelestialBodies &bodies) {
    // std::chrono::duration<double, std::micro> total_time(0);
    // auto start = std::chrono::high_resolution_clock::now();
    octree->query_region(overlap_node, resolve_collision, bodies, bodies);
    // auto end = std::chrono::high_resolution_clock::now();
    // total_time = end - start;
    // std::cout << std::endl << "Tiempo de colisiones en microsegundos : "
    // << total_time.count() << std::endl;
}

bool overlap_body(const Vec3 &center1, const Vec3 &center2, const double radius1, const double radius2) {
    double distance = (center2 - center1).magnitude();
    return radius1 + radius2 - distance >= 0;
}

bool overlap_node(NodeOctree *const &node, Vec3 &bodyPosition, double bodyRadius) {
    //para determinar si el cuerpo atraviesa parcialmente el espacio en otro nodo...
    Vec3 closestPoint = closest_point(node->get_node_center(), node->get_node_size(),
        bodyPosition);
    double distance = closestPoint.distance(bodyPosition);
    if (distance <= bodyRadius) return true;
    return false;
}

// bool overlap(NodeOctree *const &node,  CelestialBody *const& nodeBody,  CelestialBody*const& body) {
//     if (!overlap_node(node->get_node_center(), node->get_node_size(),
//                       body->get_position(), body->get_radius()))
//         return false;
//     if (nodeBody and body)
//         return overlap_body(nodeBody->get_position(), body->get_position(),
//                             nodeBody->get_radius(), body->get_radius());
//     return false;
// }

//el punto más cercano de un nodo a un cuerpo
Vec3 closest_point(const Vec3 &nodeCenter, double nodeSize, const Vec3 &bodyCenter) {
    Vec3 closestPoint;
    if (nodeCenter.get_x() - nodeSize/2 < bodyCenter.get_x() and
        nodeCenter.get_x() + nodeSize/2 > bodyCenter.get_x())
        closestPoint.set_x(bodyCenter.get_x());
    else if (nodeCenter.get_x() < bodyCenter.get_x())
        closestPoint.set_x(nodeCenter.get_x() + nodeSize/2);
    else closestPoint.set_x(nodeCenter.get_x() - nodeSize/2);

    if (nodeCenter.get_y() - nodeSize/2 < bodyCenter.get_y() and
        nodeCenter.get_y() + nodeSize/2 > bodyCenter.get_y())
        closestPoint.set_y(bodyCenter.get_y());
    else if (nodeCenter.get_y() < bodyCenter.get_y())
        closestPoint.set_y(nodeCenter.get_y() + nodeSize/2);
    else closestPoint.set_y(nodeCenter.get_y() - nodeSize/2);

    if (nodeCenter.get_z() - nodeSize/2 < bodyCenter.get_z() and
            nodeCenter.get_z() + nodeSize/2 > bodyCenter.get_z())
        closestPoint.set_z(bodyCenter.get_z());
    else if (nodeCenter.get_z() < bodyCenter.get_z())
        closestPoint.set_z(nodeCenter.get_z() + nodeSize/2);
    else closestPoint.set_z(nodeCenter.get_z() - nodeSize/2);

    return closestPoint;
}
//
// Vec3 closest_point(NodeOctree<CelestialBody> *const &node, double nodeSize, const Vec3 &bodyCenter) {
//     Vec3 closestPoint;
//     if (node->get_frontier_x_min() < bodyCenter.get_x() and
//         node->get_frontier_x_max() > bodyCenter.get_x())
//         closestPoint.set_x(bodyCenter.get_x());
//     else if (node->get_node_centerX() < bodyCenter.get_x())
//         closestPoint.set_x(node->get_frontier_x_max());
//     else closestPoint.set_x(node->get_frontier_x_min());
//
//     if (node->get_frontier_y_min() < bodyCenter.get_y() and
//         node->get_frontier_y_max() > bodyCenter.get_y())
//         closestPoint.set_y(bodyCenter.get_y());
//     else if (node->get_node_centerY() < bodyCenter.get_y())
//         closestPoint.set_y(node->get_frontier_y_max());
//     else closestPoint.set_y(node->get_frontier_y_min());
//
//     if (node->get_frontier_z_min() < bodyCenter.get_z() and
//         node->get_frontier_z_max() > bodyCenter.get_z())
//         closestPoint.set_z(bodyCenter.get_z());
//     else if (node->get_node_centerZ() < bodyCenter.get_z())
//         closestPoint.set_z(node->get_frontier_z_max());
//     else closestPoint.set_z(node->get_frontier_z_min());
//
//     return closestPoint;
// }

/*  Modelo de colisiones obtenido de:
 *  Leinhardt, Z. M., & Stewart, S. T. (2012).
 * "Collisions between gravity-dominated bodies. I. Outcome regimes and scaling laws".
 * The Astrophysical Journal, 745(1), 79.
 */

//Algoritmo adaptado de: A.1. A General Formulation for Collision Outcomes de Leinhardt & Stewart (2012)
void simplified_resolve_collision(CelestialBodies &bodies,
    int body1Index, double body1Mass, double body1Radius, Vec3 &body1Position, Vec3& body1Velocity,
    int body2Index, double body2Mass, double body2Radius, Vec3 &body2Position, Vec3& body2Velocity) {
    if (body1Mass <= 0 || body2Mass <= 0) return;
    if (body1Radius <= 0 || body2Radius <= 0) return;
    bool overlap = overlap_body(body1Position, body2Position,
        body1Radius, body2Radius);
    if (!overlap) return;
    Vec3 normal = (body2Position - body1Position).normalize();
    Vec3 relVel = body2Velocity - body1Velocity;
    if (relVel.dot(normal) >= 0) {
        return; //significa que se están separando
    }

    int largestBody = (body1Mass >= body2Mass) ? body1Index : body2Index; //body1 va a ser el mas grande
    int smallestBody = (body1Mass >= body2Mass) ? body2Index : body1Index;
    body1Mass = bodies.get_mass(largestBody);
    body1Radius = bodies.get_radius(largestBody);
    body1Position = bodies.get_position(largestBody);
    body1Velocity = bodies.get_velocity(largestBody);

    body2Mass = bodies.get_mass(smallestBody);
    body2Radius = bodies.get_radius(smallestBody);
    body2Position = bodies.get_position(smallestBody);
    body2Velocity = bodies.get_velocity(smallestBody);

    if (body1Radius + body2Radius
        - (body1Position - body2Position).magnitude() >= 0)
        //std::cout << "colision detectada" <<std::endl;
    double totalMass = body2Mass + body1Mass;
    const double relativeVelocity = (body1Velocity - body2Velocity).magnitude();
    double massInteract = mass_interact(
        body1Mass, body1Radius, body1Position, body1Velocity,
        body2Mass, body2Radius, body2Position, body2Velocity);
    //std::cout << "masa a interactuar: " << massInteract << std::endl;
    double density1 = density_by_mass_and_radius(body1Mass, body1Radius);
    double density2 = density_by_mass_and_radius(body2Mass, body2Radius);
    double avgDensity = (density1 + density2)/2;
    const double mutualEscapeVelMod = mutual_escape_velocity_mod(
        body1Mass, massInteract, avgDensity);


    //básicamente un merge o hit-and-run
    if (relativeVelocity < mutualEscapeVelMod) merge_regime(bodies,
        body1Index, body1Mass, body1Radius, body1Velocity,
        body2Index, body2Mass, body1Velocity);
    else {
        double collisionAngle = collision_angle(body1Velocity,
        body2Velocity, body1Position, body2Position);
        double bParameter = sin(collisionAngle);
        double bCrit = body1Radius/(body1Radius + body2Radius);
        bool grazingImpact = true;
        if (bParameter < bCrit) grazingImpact = false;
        if (grazingImpact)
        hit_and_run_regime(largestBody, smallestBody, massInteract, relativeVelocity,
            bParameter, avgDensity);
        else merge_regime(largestBody, smallestBody, bodies);
    }

}

//Algoritmo adaptado de: A.1. A General Formulation for Collision Outcomes de Leinhardt & Stewart (2012)
void resolve_collision(CelestialBodies *&body1, CelestialBodies *&body2, std::vector<CelestialBodies *> &bodies) {
    if (body1->get_mass() <= 0 || body2->get_mass() <= 0) return;
    if (body1->get_radius() <= 0 || body2->get_radius() <= 0) return;
    bool overlap = overlap_body(body1->get_position(), body2->get_position(),
        body1->get_radius(), body2->get_radius());
    if (!overlap) return;
    Vec3 normal = (body2->get_position() - body1->get_position()).normalize();
    Vec3 relVel = body2->get_velocity() - body1->get_velocity();
    if (relVel.dot(normal) >= 0) {
        return; //significa que se están separando
    }

    CelestialBodies *largestBody = (body1->get_mass() >= body2->get_mass()) ? body1 : body2;
    CelestialBodies *smallestBody = (body1->get_mass() >= body2->get_mass()) ? body2 : body1;
    if (largestBody->get_radius() + smallestBody->get_radius()
        - (largestBody->get_position() - smallestBody->get_position()).magnitude() >= 0)
        std::cout << "colision detectada" <<std::endl;
    double totalMass = smallestBody->get_mass() + largestBody->get_mass();
    const double relativeVelocity = (largestBody->get_velocity()
        -smallestBody->get_velocity()).magnitude();
    double massInteract = mass_interact(largestBody, smallestBody);
    std::cout << "masa a interactuar: " << massInteract << std::endl;
    double density1 = density_by_mass_and_radius(largestBody->get_mass(),
    largestBody->get_radius());
    double density2 = density_by_mass_and_radius(smallestBody->get_mass(),
        smallestBody->get_radius());
    double avgDensity = (density1 + density2)/2;
    const double mutualEscapeVelMod = mutual_escape_velocity_mod(
        largestBody->get_mass(), massInteract, avgDensity);

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
    double criticalImpVelMod = critical_impact_velocity_mod(combinedRadius, avgDensity);
    double disruptionCurve = disruption_curve(combinedRadius, avgDensity);
    double reducedMass = smallestBody->get_mass() * largestBody->get_mass() / totalMass;
    double reducedMassMod = massInteract * largestBody->get_mass()/
                            (massInteract + largestBody->get_mass());
    double disruptionCriterion = disruption_criterion(disruptionCurve,
        smallestBody->get_mass()/largestBody->get_mass());
    double criticalImpVel = critical_impact_velocity(criticalImpVelMod,
        smallestBody->get_mass()/largestBody->get_mass());
    double disruptionEnergy = disruption_energy_by_angle(disruptionCriterion,
        reducedMass, reducedMassMod);
    double criticalImpVelByAngle = impact_velocity(disruptionEnergy,
        totalMass, reducedMass);
        double specificImpEnergyErosion = specific_impact_energy(
        largestBody->get_mass(), disruptionEnergy,
        largestBody->get_mass(), smallestBody->get_mass());

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
        else hit_and_run_regime(largestBody, smallestBody, massInteract, relativeVelocity,
            bParameter, avgDensity);
    }
}

//Régimen de fusión obtenido de: Leinhardt & Stewart (2012)
void merge_regime(CelestialBodies &bodies,
    int largestBodyIndex, double largestBodyMass, double largestBodyRadius, Vec3 &largestBodyVel,
    int smallestBodyIndex, double smallestBodyMass, Vec3 &smallestBodyVel) {
    //se debe conservar el momento lineal
    Vec3 initialMomentum = momentum(largestBodyVel, largestBodyMass,
        smallestBodyVel, smallestBodyMass);
    double density = density_by_mass_and_radius(largestBodyMass, largestBodyRadius);
    bodies.set_mass(largestBodyMass + smallestBodyMass, largestBodyIndex);//absorbe al otro cuerpo
    bodies.set_velocity(initialMomentum/largestBodyMass, largestBodyIndex);//conservación el momentum
    double newRadius = radius_by_density_and_mass(largestBodyMass, density);//se ajusta el nuevo radio
    bodies.set_radius(newRadius, largestBodyIndex);
    delete_body(bodies, smallestBodyIndex);
    //como la función anterior solo borra el cuerpo del vector entonces ahora el puntero
    //smallesBody apunta a un cuerpo que no está en el vector pero aun EXISTE.
    //Para evitar llamar a una función del octree y limpiar este cuerpo lo más rápido
    //es setear sus valores en 0 y que en la siguiente reinserción de cuerpos en el arbol
    bodies.set_mass(0, smallestBodyIndex);
    bodies.set_velocity(Vec3(0, 0, 0), smallestBodyIndex);
    bodies.set_radius(0, smallestBodyIndex);
    bodies.set_force(Vec3(0, 0, 0), smallestBodyIndex);
}

//Régimen super catastrófico obtenido de: Leinhardt & Stewart (2012)
void super_catastrophic_disruption_regime(CelestialBodies *largestBody,
    CelestialBodies *smallestBody, double specificImpEnergySC,
    double disruptionEnergy, double bParameter) {
    double mLR = mass_largest_remnant_supcat(specificImpEnergySC, disruptionEnergy,
        largestBody->get_mass() + smallestBody->get_mass());
    compute_remnant_properties_and_velocities(largestBody, smallestBody, bParameter,
        mLR, 1, 2);
}

//Régimen de distrupción obtenido de: Leinhardt & Stewart (2012)
void disruption_regime(CelestialBodies *largestBody, CelestialBodies *smallestBody,
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

//Régimen hit-and-run obtenido de: Leinhardt & Stewart (2012)
void hit_and_run_regime( double largestBodyMass, double largestBodyRadius, Vec3 &largestBodyVel,
    double smallestBodyMass, double smallestBodyRadius, Vec3 &smallestBodyVel,
    double massInteract, double impactVelocity, double bParameter, double avgDensity) {
    //a pesar de que los nombres son similares a las variables que se calcularon antes en realidad
    //este régimen tiene sus propias variables
    double density = density_by_mass_and_radius(smallestBodyMass, smallestBodyRadius);
    double combRadius = radius_by_density_and_mass(smallestBodyMass + massInteract, density);
    double disruptionCurve = disruption_curve(combRadius, avgDensity);
    double critImpVelMod = critical_impact_velocity_mod(combRadius, avgDensity);
    double reducedMass = smallestBodyMass*massInteract/
        (smallestBodyMass + massInteract);
    double relationMass = massInteract / smallestBodyMass;
    double disruptionCriterion = disruption_criterion(disruptionCurve, relationMass);
    double critImpVel = critical_impact_velocity(critImpVelMod, relationMass);
    double specificImpEnergy = specific_impact_energy(reducedMass, impactVelocity,
        smallestBodyMass + massInteract);
    double massLargestRemnant = mass_largest_remnant(specificImpEnergy,
        disruptionCriterion, smallestBodyMass + massInteract);
    if (relationMass < 1.1 and relationMass > 0.9) {
        //usar N1 = 2 y N2 = 4
        compute_remnant_properties_and_velocities(
            largestBodyMass, largestBodyRadius, largestBodyVel,
            smallestBodyMass, smallestBodyRadius, smallestBodyVel,
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

//Adaptado de: Leinhardt & Stewart (2012). Ecuación (53)
double mutual_escape_velocity_mod(double largestMass, double massInteract, double avgDensity) {
    const double R = pow(3*(largestMass + massInteract)/(4 * std::numbers::pi * avgDensity), 1.0/3);
    return sqrt(2*units::G*(largestMass + massInteract)/R);
}


double catastrophic_disruption_criterion(const Vec3 &vel1, const Vec3 &vel2, double combinedRadius,
    double avgDensity) {
    // se asume que el régimen de fuerza es despreciable para satélites, planetas y demás cuerpos más masivos...

    const Vec3 relativeVelocity = vel1-vel2;
    //POR AHORA se va a asumir que los cuerpos son de densidades iguales
    const double q_g = 1; //coeficiente adimensional con valor cercano a 1
    const double density = avgDensity; //densidad arbitraria entre los dos cuerpos
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
    double cos_angle = relativeVelocity.dot(distance)/(relativeVelocity.magnitude()*distance.magnitude());
    //cos_angle = (cos_angle < 0) ? -cos_angle : cos_angle;
    cos_angle = (cos_angle <= -1)? -1: cos_angle;
    cos_angle = (cos_angle >= 1)? 1: cos_angle;
    double angle = acos(cos_angle);
    return angle;
}

//Adaptado de: Leinhardt & Stewart (2012). Ecuación (10)
double mass_interact(double largestBodyMass, double largestBodyRadius, Vec3 &largestBodyPosition, Vec3 &largestBodyVelocity,
    double smallestBodyMass, double smallestBodyRadius, Vec3 &smallestBodyPosition, Vec3 &smallestBodyVelocity) {
    double collisionAngle = collision_angle(
        largestBodyVelocity,
        smallestBodyVelocity,
        largestBodyPosition,
        smallestBodyPosition
        );
    double l_parameter = largestBodyRadius == smallestBodyRadius?
    (largestBodyPosition - smallestBodyPosition).magnitude()* (1 - sin(collisionAngle))
    :
    (largestBodyRadius - smallestBodyRadius)*(1 - sin(collisionAngle));
    double density1 = density_by_mass_and_radius(largestBodyMass, largestBodyRadius);
    double density2 = density_by_mass_and_radius(smallestBodyMass, smallestBodyRadius);
    double avgDensity = (density1 + density2)/2;
    return avgDensity * std::numbers::pi * l_parameter*l_parameter *
        (smallestBodyRadius - l_parameter/3.0);
}

//Obtenido de: Leinhardt & Stewart (2012). Ecuación (28)
double disruption_curve(double combinedRadius, double avgDensity) {
    double c = 1.9; //Representa la diferencia entre la energía de
        //enlace gravitacional y el criterio de disipación para masas iguales. El valor recomendado
        //para cuerpos como planetas es de 1.9+-0.3
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

void refresh_body(double currentMass, double newMass, double currentRadius,
    const Vec3 &newVelocity, CelestialBodies *&body) {
    double density = density_by_mass_and_radius(currentMass, currentRadius);
    body->set_mass(newMass);
    double newRadius = radius_by_density_and_mass(newMass, density);
    body->set_radius(newRadius);
    body->set_velocity(newVelocity);
}

void update_bodies_after_fragmentation(CelestialBodies *&largestBody, CelestialBodies *&smallestBody,
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

    fix_positions_after_fragmentation(largestBody, smallestBody);
}

//para separar los cuerpos después de la colision
void fix_positions_after_fragmentation(CelestialBodies *&largestBody, CelestialBodies *&smallestBody) {
    double penetration = largestBody->get_radius() + smallestBody->get_radius() -
        (largestBody->get_position()-smallestBody->get_position()).magnitude();
    if (penetration > 0) {
        Vec3 normal;
        Vec3 delta = smallestBody->get_position() - largestBody->get_position();
        double dist = delta.magnitude();
        if (dist < 1e-8) normal = Vec3(1,0,0);
        else normal = delta/dist;
        double totalMass = largestBody->get_mass() + smallestBody->get_mass();

        double w1 = smallestBody->get_mass() / totalMass;
        double w2 = largestBody->get_mass() / totalMass;

        largestBody->set_position(largestBody->get_position() - normal * penetration * w1);
        smallestBody->set_position(smallestBody->get_position() + normal * penetration * w2);
    }
}

//OBtenido de: Leinhardt & Stewart (2012). Ecuación (44)
double mass_largest_remnant_supcat(double specificImpEnergySC, double disruptionEnergy,
    double totalMass) {
    const double n =-1.5; //valor aproximado que se sugiere
    const double firstTerm = 0.1/pow(1.8, n);
    return firstTerm * totalMass * pow(specificImpEnergySC/disruptionEnergy, n);
}

void compute_remnant_properties_and_velocities(
    double largestBodyMass, double largestBodyRadius, Vec3 &largestBodyVel,
    double smallestBodyMass, double smallestBodyRadius, Vec3 &smallestBodyVel,
    double bParameter, double mLR, int N1, int N2) {
    Vec3 initialMomentum = momentum(largestBodyVel, largestBodyMass,
            smallestBodyVel, smallestBodyMass);
    double mSLR = mass_second_largest_remnant(mLR,
        largestBodyMass + smallestBodyMass, N1, N2);
    Vec3 velCM = (largestBodyVel * largestBodyMass +
            smallestBodyVel * smallestBodyMass) /
                (largestBodyMass + smallestBodyMass);
    if (bParameter == 0)//frontal
        update_bodies_after_fragmentation(largestBody, smallestBody,
            mLR, mSLR, velCM, initialMomentum);
    else if (bParameter > 0.7)
        update_bodies_after_fragmentation(largestBody, smallestBody,
            mLR, mSLR, largestBody->get_velocity(), initialMomentum);
    else {
        Vec3 newVelLR;
        if (bParameter < 0.175) newVelLR = velCM * 0.8+ largestBodyVel * 0.2;
        else if (bParameter < 0.35) newVelLR = velCM * 0.6+ largestBodyVel * 0.4;
        else if (bParameter < 0.525) newVelLR = velCM * 0.4+ largestBodyVel * 0.6;
        else newVelLR = velCM * 0.2 + largestBodyVel * 0.8;
        update_bodies_after_fragmentation(largestBody, smallestBody,
            mLR, mSLR, newVelLR, initialMomentum);
    }
}