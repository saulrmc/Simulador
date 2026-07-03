//
// Created by Saúl on 21/12/2025.
//
#pragma once
#ifndef SIMULADORGRAVITACIONAL_GESTORCALCULOS_H
#define SIMULADORGRAVITACIONAL_GESTORCALCULOS_H
#include <vector>
#include "PhysicsUtils.h"
#include "spatial_structures/Octree/Octree.h"
#include "Collisions.h"
#include "LeapfrogKDK.h"

class CalculationManager {
    public:
    CalculationManager();
    virtual ~CalculationManager();
    void create_Octree();
    void reinsert_bodies(CelestialBodies &bodies);
    void update_forces(CelestialBodies &bodies);
    void step(CelestialBodies &bodies);

    void preserve_root_and_update_forces(CelestialBodies &bodies);

    void leapfrog_integration_kdk(CelestialBodies &bodies);
    void root_space(CelestialBodies &bodies);

    private:
    Octree *root;
    unsigned int numStep;
};


#endif //SIMULADORGRAVITACIONAL_GESTORCALCULOS_H