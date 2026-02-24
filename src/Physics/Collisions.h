//
// Created by Saúl on 15/02/2026.
//

#ifndef SIMULADORGRAVITACIONAL_COLLISIONS_H
#define SIMULADORGRAVITACIONAL_COLLISIONS_H
#include <vector>
#include "../Commons/CelestialBody.h"
#include "Octree/Octree.h"

void resolve_collisions(const Octree *&octree, const NodeOctree *&node,
    const std::vector<CelestialBody *> &bodies, int begin, int end);
void detect_collisions(const NodeOctree *&node, const double body_radius);
bool intersection(const Vec3 &center, const double size, const double radius);

#endif //SIMULADORGRAVITACIONAL_COLLISIONS_H

