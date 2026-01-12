//
// Created by Saúl on 21/12/2025.
//

#ifndef SIMULADORGRAVITACIONAL_GESTORCALCULOS_H
#define SIMULADORGRAVITACIONAL_GESTORCALCULOS_H
#include <vector>

#include "../Creation/CelestialBody.h"
#include "PhysicsUtils.h"
class CalculationManager {
    public:
    CalculationManager();
    virtual ~CalculationManager();
    void simulation_begin();
    void show_simulation();
    void simulation_ends();
    void add_body();
    void delete_body();

    private:
    std::vector<CelestialBody> bodies{};
};


#endif //SIMULADORGRAVITACIONAL_GESTORCALCULOS_H