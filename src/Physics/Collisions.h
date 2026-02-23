//
// Created by Saúl on 15/02/2026.
//

#ifndef SIMULADORGRAVITACIONAL_COLLISIONS_H
#define SIMULADORGRAVITACIONAL_COLLISIONS_H
#include <vector>
#include "../Commons/CelestialBody.h"
#include "Octree/Octree.h"

void detect_collisions(Octree *&octree, const std::vector<CelestialBody *> &bodies, int begin, int end);


#endif //SIMULADORGRAVITACIONAL_COLLISIONS_H

