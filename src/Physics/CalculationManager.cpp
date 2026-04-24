//
// Created by Saúl on 21/12/2025.
//

#include "CalculationManager.h"

#include <chrono>
#include <iostream>

CalculationManager::CalculationManager() {
    root = nullptr;
    numStep=0;
}

CalculationManager::~CalculationManager() {
    delete root;
}

void CalculationManager::create_Octree() {
    if (!root) root = new Octree();
    root->create_space();
}

void CalculationManager::reinsert_bodies(std::vector<CelestialBody*> &bodies) {
    if (!root) {
        root_space(bodies);
        root->create_space();
    }
    for (CelestialBody *&body : bodies) {
        root->insert(body);
    }
}


void CalculationManager::update_forces(std::vector<CelestialBody*> &bodies) {
    if (root) {
        delete root;
        root = nullptr;
    }
    root_space(bodies);
    create_Octree();
    std::chrono::duration<double, std::micro> total_time(0);
    auto start = std::chrono::high_resolution_clock::now();
    reinsert_bodies(bodies);
    root->refresh_theta_value();
    auto end = std::chrono::high_resolution_clock::now();
    total_time = end - start;
    std::cout << std::endl << "Tiempo de reinsercion de cuerpos en microsegundos : "
    << total_time.count() << std::endl;
    std::chrono::duration<double, std::micro> total_time2(0);
    auto start2 = std::chrono::high_resolution_clock::now();
    #pragma omp parallel for schedule(dynamic, 10)
    //#pragma omp parallel for schedule(dynamic, 100)
    //#pragma omp parallel for schedule(dynamic, 1000)
    //#pragma omp parallel for schedule(dynamic, 10000)
    for (CelestialBody *&body : bodies) root->calc_forces_per_body(body);//fuerzas actualizadas
    auto end2 = std::chrono::high_resolution_clock::now();
    total_time2 = end2 - start2;
    std::cout << "Tiempo de calculo de fuerzas de cuerpos en microsegundos : "
    << total_time2.count() << std::endl;
}

void CalculationManager::step(std::vector<CelestialBody *> &bodies) {
    leapfrog_integration_kdk(bodies);
    numStep++;
}

void CalculationManager::preserve_root_and_update_forces(std::vector<CelestialBody *> &bodies) {
    root->refresh_theta_value();
    root->refresh_mass_centers();
    for (CelestialBody *&body : bodies) root->calc_forces_per_body(body);//fuerzas actualizadas
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
    if (numStep % 5 == 0) update_forces(bodies);
    else preserve_root_and_update_forces(bodies);
    //update_forces(bodies);
    leapfrog_integration_kick(bodies);
}

void CalculationManager::root_space(std::vector<CelestialBody *> &bodies) {
    if (!root) root = new Octree();
    //el index 0 y 1 para xMin y xMax
    //el index 2 y 3 para yMin & yMax
    //el index 4 y 5 para zMin y zMax
    double frontierValues[6]{};
    for (CelestialBody *&body : bodies) {
        if (body->get_position().get_x() < frontierValues[0])
            frontierValues[0] = body->get_position().get_x();
        else if (body->get_position().get_x() > frontierValues[1])
            frontierValues[1] = body->get_position().get_x();

        if (body->get_position().get_y() < frontierValues[2])
            frontierValues[2] = body->get_position().get_y();
        else if (body->get_position().get_y() > frontierValues[3])
            frontierValues[3] = body->get_position().get_y();

        if (body->get_position().get_z() < frontierValues[4])
            frontierValues[4] = body->get_position().get_z();
        else if (body->get_position().get_z() > frontierValues[5])
            frontierValues[5] = body->get_position().get_z();
    }
    double exactSize = std::max(std::max(frontierValues[1] - frontierValues[0],
        frontierValues[3] - frontierValues[2]),
            frontierValues[5] - frontierValues[4]);
    double nearestPow2 = log2(exactSize);
    int minPow2 = (int)nearestPow2;
    int maxPow2 = minPow2 + 1;

    root->set_size(pow(2, maxPow2));
    // std::cout << exactSize << std::endl;
    // std::cout << root->get_size()<<std::endl;
    root->set_center(Vec3((frontierValues[0] + frontierValues[1])/2,
        (frontierValues[2] + frontierValues[3])/2,
        (frontierValues[4] + frontierValues[5])/2));
}