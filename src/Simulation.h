//
// Created by Saúl on 17/01/2026.
//

#ifndef SIMULADORGRAVITACIONAL_SIMULATION_H
#define SIMULADORGRAVITACIONAL_SIMULATION_H
#include "Creation/Creation.h"
#include "Physics/CalculationManager.h"


class Simulation {
    public:
    Simulation();
    virtual ~Simulation();
    void run_simulation();
    private:
    CalculationManager calcManager{};
    std::vector<CelestialBody *> bodies{};

};
void create_many_bodies(std::vector<CelestialBody*>& bodies, int N) ;

#endif //SIMULADORGRAVITACIONAL_SIMULATION_H