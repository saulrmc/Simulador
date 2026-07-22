//
// Created by Saúl on 21/12/2025.
//
#pragma once
#ifndef SIMULADORGRAVITACIONAL_GESTORCALCULOS_H
#define SIMULADORGRAVITACIONAL_GESTORCALCULOS_H
#include <vector>
#include "PhysicsUtils.h"
#include "Collisions.h"
#include "LeapfrogKDK.h"

#ifdef USE_LINEAR_OCTREE
#include "spatial_structures/LinearOctree/LinearOctree.h"
using SpatialOctree = LinearOctree;
#else
#include "spatial_structures/Octree/Octree.h"
using SpatialOctree = Octree;
#endif

class CalculationManager {
    public:
    CalculationManager();
    ~CalculationManager();
    void create_structure();
    void reinsert_bodies(CelestialBodies &bodies);
    void update_forces(CelestialBodies &bodies);
    void step(CelestialBodies &bodies);

    void preserve_root_and_update_forces(CelestialBodies &bodies);

    void leapfrog_integration_kdk(CelestialBodies &bodies);
    void root_space(CelestialBodies &bodies);

    private:
    SpatialOctree *root;
    unsigned int numStep;
};


#endif //SIMULADORGRAVITACIONAL_GESTORCALCULOS_H
