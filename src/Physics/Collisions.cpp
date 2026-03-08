//
// Created by Saúl on 15/02/2026.
//

#include "Collisions.h"

void collisions_for_bodies(const Octree *&octree, const NodeOctree *&node,
    const std::vector<CelestialBody *> &bodies, int begin, int end) {
    if (begin > end) return;
    if (begin == end) {

        octree->query_region(node, intersection, collision);
    }
    int middle = begin + (end - begin) / 2;
    collisions_for_bodies(octree, node, bodies, begin, middle);
    collisions_for_bodies(octree, node, bodies, middle + 1,  end);
}

bool intersection(const Vec3 &center1, const Vec3 &center2, const double radius1, const double radius2) {
    double distance = (center2 - center1).magnitude();
    return radius1 + radius2 <= distance;
}


