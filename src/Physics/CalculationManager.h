//
// Created by Saúl on 21/12/2025.
//
#pragma once
#ifndef SIMULADORGRAVITACIONAL_GESTORCALCULOS_H
#define SIMULADORGRAVITACIONAL_GESTORCALCULOS_H
#include <vector>
#include "PhysicsUtils.h"
#include "Octree/Octree.h"
#include "Collisions.h"
#include "LeapfrogKDK.h"

class CalculationManager {
    public:
    CalculationManager();
    virtual ~CalculationManager();
    void create_Octree();
    void reinsert_bodies(std::vector<CelestialBody *> &);
    void update_forces(std::vector<CelestialBody*> &);
    void step(std::vector<CelestialBody *> &);

    void preserve_root_and_update_forces(std::vector<CelestialBody *> & bodies);

    void leapfrog_integration_kdk(std::vector<CelestialBody *> &);
    void root_space(std::vector<CelestialBody *> &bodies);

    private:
    Octree *root;
    unsigned int numStep;
};


#endif //SIMULADORGRAVITACIONAL_GESTORCALCULOS_H