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
    double collisionAngle = collision_angle(body1->get_velocity(),
        body2->get_velocity(), body1->get_position(),
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
    double specificImpEnergySC = specific_impact_energy(0.1 * totalMass,
    disruptionEnergy, largestBody->get_mass(), smallestBody->get_mass());
    double vSupercat = impact_velocity(reducedMass, specificImpEnergySC, totalMass);

    if (relativeVelocity > vSupercat) super_catastrophic_disruption_regime(largestBody, smallestBody);
    else {
        if (!grazingImpact) {
            if (relativeVelocity > vErosion) erosion_disruption_regime_non_grazing(largestBody, smallestBody);
            else partial_acretion_regime(largestBody, smallestBody);
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