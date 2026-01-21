//
// Created by Saúl on 21/12/2025.
//

#include "CalculationManager.h"

CalculationManager::CalculationManager() {
    root = nullptr;
}

CalculationManager::~CalculationManager() {
    delete root;
}

void CalculationManager::create_Octree() {
    root->create_space();
}

void CalculationManager::reinsert_nodes(std::vector<CelestialBody> &bodies) {
    for (CelestialBody &body : bodies) {
        root->insert(&body);
    }
}

void CalculationManager::leapfrog_integration_kick(std::vector<CelestialBody> &bodies) {
    for (CelestialBody &body : bodies) {
        Vec3 next_velocity = next_velocity_for_delta_time(units::HALF_DELTA_TIME,
            body.get_mass(), body.get_force(), body.get_velocity());
        body.set_velocity(next_velocity);
    }
}

void CalculationManager::leapfrog_integration_drift(std::vector<CelestialBody> &bodies) {
    for (CelestialBody &body : bodies) {
        Vec3 next_position = next_position_for_delta_time(units::DELTA_TIME, body.get_velocity(), body.get_position());
        body.set_position(next_position);
    }
}

void CalculationManager::update_forces(std::vector<CelestialBody> &bodies) {
    delete root;
    create_Octree();
    reinsert_nodes(bodies);
    for (CelestialBody &body : bodies) root->calc_forces_per_body(&body);//fuerzas actualizadas
}

void CalculationManager::step(std::vector<CelestialBody> &bodies) {
    //este codigo debe implementar una inicializacion de las fuerzas antes de ser llamado
    //porque asume que todos los cuerpos ya tienen las fuerzas inicializadas/actualizadas
    //este código debería ir antes del while principal del programa:
    //update_forces(bodies);

    leapfrog_integration_kick(bodies); //avanza de t a t + dt/2 pero solo las velocidades
    leapfrog_integration_drift(bodies); //avanza de t a t + dt y se actualiza el resto de atributos
    //como las posiciones se actualizaron entonces el octree se debe actualizar
    //porque solo representa un instante t de tiempo
    update_forces(bodies);
    leapfrog_integration_kick(bodies);
}


