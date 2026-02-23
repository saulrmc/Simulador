//
// Created by Saúl on 15/02/2026.
//

#include "Collisions.h"

void resolve_collisions(Octree *&octree, const std::vector<CelestialBody *> &bodies, int begin, int end) {
    if (begin > end) return;
    if (begin == end) {
        NodeOctree *bodyFatherNode = octree->locate_node_father(bodies[begin]);
        if (!bodyFatherNode) return; //significa que es el único cuerpo en la simulación
        for (int i = 0; i < 8; i++) {
            //detectas colision

            //resuelves colision
        }
    }
    int middle = begin + (end - begin) / 2;
    detect_collisions(octree, bodies, begin, middle);
    detect_collisions(octree, bodies, middle + 1,  end);
}
