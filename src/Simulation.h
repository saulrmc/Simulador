//
// Created by Saúl on 17/01/2026.
//

#ifndef SIMULADORGRAVITACIONAL_SIMULATION_H
#define SIMULADORGRAVITACIONAL_SIMULATION_H
#include "Commons/CelestialBody.h"
#include "Physics/CalculationManager.h"


class Simulation {
    public:
    Simulation();
    virtual ~Simulation();
    void run_simulation();
    private:
    std::vector<CelestialBody> bodies{};
    CalculationManager calcManager{};

};


#endif //SIMULADORGRAVITACIONAL_SIMULATION_H