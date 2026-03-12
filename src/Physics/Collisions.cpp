//
// Created by Saúl on 15/02/2026.
//

#include "Collisions.h"

void collisions_for_bodies(const Octree *&octree, const NodeOctree *&node,
    const std::vector<CelestialBody *> &bodies, int begin, int end) {
    if (begin > end) return;
    if (begin == end) {
        octree->query_region(node, overlap_node, resolve_collision, bodies[begin]);
    }
    int middle = begin + (end - begin) / 2;
    collisions_for_bodies(octree, node, bodies, begin, middle);
    collisions_for_bodies(octree, node, bodies, middle + 1,  end);
}

bool overlap_body(const Vec3 &center1, const Vec3 &center2, const double radius1, const double radius2) {
    double distance = (center2 - center1).magnitude();
    return radius1 + radius2 <= distance;
}

bool overlap_node(Vec3 &nodeCenter, double nodeSize, Vec3 &bodyCenter, double bodyRadius) {
    //para determinar si el cuerpo atraviesa parcialmente el espacio en otro nodo...
    Vec3 closestPoint = closest_point(nodeCenter, nodeSize, bodyCenter);
    double distance = closestPoint.distance(bodyCenter);
    if (distance <= bodyRadius) return true;
    return false;
}

//el punto más cercano de un nodo a un cuerpo
Vec3 closest_point(Vec3 &nodeCenter, double nodeSize, Vec3 &bodyCenter) {
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

void resolve_collision(CelestialBody *otherBody, CelestialBody *currentBody) {
    if (!overlap_body(otherBody->get_position(), currentBody->get_position(),
        otherBody->get_radius(), currentBody->get_radius())) return;

    const double relativeVelocity = (otherBody->get_velocity()-currentBody->get_velocity()).magnitude();
    const double mutualEscapeVel = mutual_escape_velocity(otherBody->get_mass(),
        currentBody->get_mass(), otherBody->get_position(), currentBody->get_position());

    double effectiveEnergy =
        effective_specific_impact_energy(otherBody->get_mass(), currentBody->get_mass(),
                                    otherBody->get_velocity(), currentBody->get_velocity(),
                                    otherBody->get_position(), currentBody->get_position());

    double catastrophicCriterion = catastrophic_disruption_criterion(otherBody->get_velocity(),
        currentBody->get_velocity(), otherBody->get_radius(), currentBody->get_radius());

    double massLargestRem = mass_largest_remmant(otherBody->get_mass(),
        currentBody->get_mass(), effectiveEnergy, catastrophicCriterion);

    if (otherBody->get_mass()/currentBody->get_mass() < 0.01 or
        currentBody->get_mass()/otherBody->get_mass() < 0.01) {
        fusion(otherBody, currentBody);
    }
    else if (massLargestRem/(otherBody->get_mass() + currentBody->get_mass()) < 0.9 or
        massLargestRem/(otherBody->get_mass() + currentBody->get_mass()) > 0.1 or
        relativeVelocity > mutualEscapeVel) {
        catastrophic_disruption(otherBody, currentBody);
    }


}