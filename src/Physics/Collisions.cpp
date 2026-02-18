//
// Created by Saúl on 15/02/2026.
//

#include "Collisions.h"

void detect_collisions(Octree *&octree, const std::vector<CelestialBody *> &bodies, std::vector<CelestialBody *> &collidingBodies,
    int begin, int end) {
    if (begin > end) return;
    if (begin == end) {
        NodeOctree *bodyFatherNode = octree->locate_node_father(bodies[begin]);
        if (!bodyFatherNode) return; //significa que es el único cuerpo en la simulación
        for (int i=0;i<8;i++) {
            if (collision(, bodies[begin])) collidingBodies.push_back(); //queda pendiente verificar que no se agreguen
            //elementos repetidos y además cómo hacer que el nodo de alguna manera retorne el puntero al cuerpo
            //sin romper con el encapsulamiento
        }
    }
    int middle = begin + (end - begin) / 2;
    detect_collisions(octree, bodies, collidingBodies, begin, end);
    detect_collisions(octree, bodies, collidingBodies, middle + 1,  end);
}
void resolve_collisions() {

}
