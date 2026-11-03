//
// Created by Saúl on 21/12/2025.
//
#pragma once
#ifndef SIMULADORGRAVITACIONAL_GESTORCALCULOS_H
#define SIMULADORGRAVITACIONAL_GESTORCALCULOS_H
#include <string>
#include <vector>
#include "PhysicsUtils.h"
#include "Collisions.h"
#include "LeapfrogKDK.h"

#include "spatial_structures/config.h"

#if USE_LINEAR_OCTREE
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

    // Tiempos de la última llamada a update_forces / leapfrog_integration_kdk,
    // en microsegundos. Los usan los benchmarks.
    double insTime, calcForcTime, kickTime, driftTime, collTime,
           uptMassTime, crtSpcTime, delTime, bboxTime;

    double get_theta() const { return root ? root->get_theta() : 0.0; }
    const std::string& get_structure_name() const {
        static const std::string unknown = "None";
        return root ? root->get_name() : unknown;
    }

private:
    SpatialOctree *root;
    unsigned int numStep;
};


#endif //SIMULADORGRAVITACIONAL_GESTORCALCULOS_H
