//
// Created by Saúl on 15/02/2026.
//

#include "Collisions.h"

void resolve_collisions(const Octree *&octree, const NodeOctree *&node,
    const std::vector<CelestialBody *> &bodies, int begin, int end) {
    if (begin > end) return;
    if (begin == end) {
        detect_collisions(node, bodies[begin]->get_radius());
        octree->query_region(node, intersection, collision);
    }
    int middle = begin + (end - begin) / 2;
    resolve_collisions(octree, node, bodies, begin, middle);
    resolve_collisions(octree, node, bodies, middle + 1,  end);
}

bool intersection(const Vec3 &center, const double size, const double radius) {

}
void collision(double effective_energy, double grav_biding_energy) {
    if (effective_energy > grav_biding_energy) ;

}