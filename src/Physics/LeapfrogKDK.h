//
// Created by Saúl on 20/03/2026.
//

#ifndef SIMULADORGRAVITACIONAL_LEAPFROGKDK_H
#define SIMULADORGRAVITACIONAL_LEAPFROGKDK_H
#include "../Commons/CelestialBody.h"
#include "Physics.h"
#include <vector>
#endif //SIMULADORGRAVITACIONAL_LEAPFROGKDK_H


void leapfrog_integration_kick(std::vector<CelestialBody *> &);
void leapfrog_integration_drift(std::vector<CelestialBody *> &);