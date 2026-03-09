//
// Created by Saúl on 15/02/2026.
//

#include "Collisions.h"

void collisions_for_bodies(const Octree *&octree, const NodeOctree *&node,
    const std::vector<CelestialBody *> &bodies, int begin, int end) {
    if (begin > end) return;
    if (begin == end) {
        octree->query_region(node, overlap_node, collision);
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