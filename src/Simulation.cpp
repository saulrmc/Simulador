//
// Created by Saúl on 17/01/2026.
//

#include "Simulation.h"

Simulation::Simulation() {
    calcManager = CalculationManager();
}

Simulation::~Simulation() {
}

void Simulation::run_simulation() {
    calcManager.update_forces(bodies);
    while (true) {
        calcManager.step(bodies);
    }
}
