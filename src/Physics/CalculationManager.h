//
// Created by Saúl on 21/12/2025.
//
#pragma once
#ifndef SIMULADORGRAVITACIONAL_GESTORCALCULOS_H
#define SIMULADORGRAVITACIONAL_GESTORCALCULOS_H
#include <vector>
#include "PhysicsUtils.h"
#include "Octree/Octree.h"

class CalculationManager {
    public:
    CalculationManager();
    virtual ~CalculationManager();
    void create_Octree();
    void reinsert_nodes(const std::vector<CelestialBody *> &);
    void leapfrog_integration_kick(std::vector<CelestialBody *> &);
    void leapfrog_integration_drift(std::vector<CelestialBody *> &);
    void update_forces(std::vector<CelestialBody*> &);
    void step(std::vector<CelestialBody *> &);
    void check_collisions(CelestialBody *, Vec3 &);

    private:
    Octree *root;
};


#endif //SIMULADORGRAVITACIONAL_GESTORCALCULOS_H