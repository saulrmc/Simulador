//
// Created by Saúl on 20/03/2026.
//

#ifndef SIMULADORGRAVITACIONAL_LEAPFROGKDK_H
#define SIMULADORGRAVITACIONAL_LEAPFROGKDK_H
#include "../Commons/CelestialBodies.h"
#include "Physics.h"
#include <vector>

void leapfrog_integration_kick(CelestialBodies &);
void leapfrog_integration_drift(CelestialBodies &);

#endif //SIMULADORGRAVITACIONAL_LEAPFROGKDK_H