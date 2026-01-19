//
// Created by Saúl on 21/12/2025.
//

#ifndef SIMULADORGRAVITACIONAL_GESTORCALCULOS_H
#define SIMULADORGRAVITACIONAL_GESTORCALCULOS_H
#include <vector>

#include "../Commons/CelestialBody.h"
#include "PhysicsUtils.h"
#include "Octree/Octree.h"

class CalculationManager {
    public:
    CalculationManager();
    virtual ~CalculationManager();
    void step();
    void create_Octree();
    void reinsert_nodes(std::vector<CelestialBody> &);

    private:
    Octree *root;
};


#endif //SIMULADORGRAVITACIONAL_GESTORCALCULOS_H