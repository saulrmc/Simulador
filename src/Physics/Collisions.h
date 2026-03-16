//
// Created by Saúl on 15/02/2026.
//

#ifndef SIMULADORGRAVITACIONAL_COLLISIONS_H
#define SIMULADORGRAVITACIONAL_COLLISIONS_H
#include <vector>
#include "../Commons/CelestialBody.h"
#include "Octree/Octree.h"

void collisions_for_bodies(Octree *const &octree, NodeOctree *const &node,
    const std::vector<CelestialBody *> &bodies, int begin, int end);
double overlap_body(const Vec3 &center1, const Vec3 &center2, const double radius1, const double radius2) ;
bool overlap_node(const Vec3 &nodeCenter, double nodeSize, const Vec3 &bodyCenter, double bodyRadius) ;
Vec3 closest_point(const Vec3 &nodeCenter, double nodeSize, const Vec3 &bodyCenter) ;
void resolve_collision(CelestialBody *&, CelestialBody *&);
#endif //SIMULADORGRAVITACIONAL_COLLISIONS_H

