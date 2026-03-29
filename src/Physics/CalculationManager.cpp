//
// Created by Saúl on 21/12/2025.
//

#include "CalculationManager.h"

#include <chrono>
#include <iostream>

CalculationManager::CalculationManager() {
    root = nullptr;
}

CalculationManager::~CalculationManager() {
    delete root;
}

void CalculationManager::create_Octree() {
    if (!root) root = new Octree();
    root->create_space();
}

void CalculationManager::reinsert_bodies(std::vector<CelestialBody*> &bodies) {
    for (CelestialBody *&body : bodies) {
        root->insert(body);
    }
}


void CalculationManager::update_forces(std::vector<CelestialBody*> &bodies) {
    if (root) {
        delete root;
        root = nullptr;
    }
    create_Octree();
    std::chrono::duration<double, std::micro> total_time(0);
    auto start = std::chrono::high_resolution_clock::now();
    reinsert_bodies(bodies);
    root->refresh_theta_value();
    auto end = std::chrono::high_resolution_clock::now();
    total_time = end - start;
    std::cout << std::endl << "Tiempo de reinsercion de cuerpos en microsegundos : "
    << total_time.count() << std::endl;
    for (CelestialBody *&body : bodies) root->calc_forces_per_body(body);//fuerzas actualizadas
}

void CalculationManager::step(std::vector<CelestialBody *> &bodies) {
    leapfrog_integration_kdk(bodies);
}

void CalculationManager::leapfrog_integration_kdk(std::vector<CelestialBody *> &bodies) {
    //este codigo debe implementar una inicializacion de las fuerzas antes de ser llamado
    //porque asume que todos los cuerpos ya tienen las fuerzas inicializadas/actualizadas
    //este código debería ir antes del while principal del programa:
    //update_forces(bodies);
    leapfrog_integration_kick(bodies); //avanza de t a t + dt/2 pero solo las velocidades
    leapfrog_integration_drift(bodies); //avanza de t a t + dt y se actualiza el resto de atributos
    //como las posiciones se actualizaron entonces el octree se debe actualizar
    //porque solo representa un instante t de tiempo

    //acá se deberían resolver las colisiones pero ojo que las velocidades actualizadas van a ser de dt/2
    //version 1:
    //collisions_for_bodies(root, bodies, 0, bodies.size() - 1);
    //versión 2:
    collisions_for_bodies(root, bodies);

    update_forces(bodies);
    leapfrog_integration_kick(bodies);
}

