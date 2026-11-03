//
// Created by Saúl on 21/12/2025.
//

#include "CalculationManager.h"

#include <chrono>
#include <iostream>

CalculationManager::CalculationManager() {
    root = new SpatialOctree();
    numStep=0;
    insTime = calcForcTime = kickTime = driftTime = collTime = 0.0;
    uptMassTime = crtSpcTime = delTime = bboxTime = 0.0;
}

CalculationManager::~CalculationManager() {
    delete root;
}

void CalculationManager::create_structure() {
    if (!root) root = new SpatialOctree();
    root->create_space();
}

void CalculationManager::reinsert_bodies(CelestialBodies &bodies) {
    if (!root) {
        root_space(bodies);
        root->create_space();
    }
    root->insert(bodies);
}


void CalculationManager::update_forces(CelestialBodies &bodies) {
    using clock = std::chrono::steady_clock;

    // Destruccion del arbol del paso anterior. En el octree de punteros es un
    // recorrido recursivo con un free() por nodo, asi que se mide aparte.
    auto phaseStart = clock::now();
    if (root) {
        delete root;
        root = nullptr;
    }
    delTime = std::chrono::duration<double, std::micro>(clock::now() - phaseStart).count();

    // Recalculo de la caja contenedora (O(N) por paso).
    phaseStart = clock::now();
    root_space(bodies);
    bboxTime = std::chrono::duration<double, std::micro>(clock::now() - phaseStart).count();

    std::chrono::duration<double, std::micro> total_time_0(0);
    auto start = std::chrono::steady_clock::now();
    create_structure();
    auto end = std::chrono::steady_clock::now();
    total_time_0 = end - start;
    crtSpcTime = total_time_0.count();

    std::chrono::duration<double, std::micro> total_time(0);
    start = std::chrono::steady_clock::now();
    reinsert_bodies(bodies);
    end = std::chrono::steady_clock::now();
    total_time = end - start;
    insTime = total_time.count();
    // std::cout << std::endl << root->get_name() << std::endl
    //                          << " Tiempo de reinsercion de cuerpos en microsegundos : "
    //                          << total_time.count() << std::endl;
    std::chrono::duration<double, std::micro> total_time4(0);
    start = std::chrono::steady_clock::now();
    root->refresh_mass_centers(bodies);
    end = std::chrono::steady_clock::now();
    total_time4 = end - start;
    uptMassTime = total_time4.count();

    root->refresh_theta_value();
    std::chrono::duration<double, std::micro> total_time2(0);
    auto start2 = std::chrono::steady_clock::now();
    //#pragma omp parallel for schedule(dynamic, 100)
    // #pragma omp parallel for schedule(dynamic, 64)
    root->calc_forces(bodies);//fuerzas actualizadas
    auto end2 = std::chrono::steady_clock::now();
    total_time2 = end2 - start2;
    calcForcTime = total_time2.count();
    // std::cout << "Tiempo de calculo de fuerzas de cuerpos en microsegundos : "
    // << total_time2.count() << std::endl;
}

void CalculationManager::step(CelestialBodies &bodies) {
    leapfrog_integration_kdk(bodies);
    numStep++;
}

void CalculationManager::preserve_root_and_update_forces(CelestialBodies &bodies) {
    root->refresh_theta_value();
    root->refresh_mass_centers(bodies);
    root->calc_forces(bodies);//fuerzas actualizadas
}

void CalculationManager::leapfrog_integration_kdk(CelestialBodies &bodies) {
    //este codigo debe implementar una inicializacion de las fuerzas antes de ser llamado
    //porque asume que todos los cuerpos ya tienen las fuerzas inicializadas/actualizadas
    //este código debería ir antes del while principal del programa:
    //update_forces(bodies);
    std::chrono::duration<double, std::micro> total_time(0);
    auto start = std::chrono::steady_clock::now();
    leapfrog_integration_kick(bodies); //avanza de t a t + dt/2 pero solo las velocidades
    auto end = std::chrono::steady_clock::now();
    total_time = end - start;
    kickTime = total_time.count();

    std::chrono::duration<double, std::micro> total_time2(0);
    start = std::chrono::steady_clock::now();
    leapfrog_integration_drift(bodies); //avanza de t a t + dt y se actualiza el resto de atributos
    //como las posiciones se actualizaron entonces el octree se debe actualizar
    //porque solo representa un instante t de tiempo
    end = std::chrono::steady_clock::now();
    total_time2 = end - start;
    driftTime = total_time2.count();
    //acá se deberían resolver las colisiones pero ojo que las velocidades actualizadas van a ser de dt/2

    std::chrono::duration<double, std::micro> total_time3(0);
    start = std::chrono::steady_clock::now();
    int deleted = collisions(root, bodies);
    end = std::chrono::steady_clock::now();
    total_time3 = end - start;
    collTime = total_time3.count();

    //  TODO: Realmente vale la pena reconstruir el árbol siempre?
    // if (numStep % 5 == 0 || deleted > 0) update_forces(bodies);
    // else preserve_root_and_update_forces(bodies);
    update_forces(bodies);

    //update_forces(bodies);
    leapfrog_integration_kick(bodies);
}

void CalculationManager::root_space(CelestialBodies  &bodies) {
    if (!root) root = new SpatialOctree();
    //el index 0 y 1 para xMin y xMax
    //el index 2 y 3 para yMin & yMax
    //el index 4 y 5 para zMin y zMax
    if (bodies.size() == 0) return;
    double frontierValues[6] {
        bodies.get_posX(0), bodies.get_posX(0),
        bodies.get_posY(0), bodies.get_posY(0),
        bodies.get_posZ(0), bodies.get_posZ(0)
    };
    for (int i = 0; i < bodies.size(); i++) {
        if (bodies.get_posX(i) < frontierValues[0])
            frontierValues[0] = bodies.get_posX(i);
        else if (bodies.get_posX(i) > frontierValues[1])
            frontierValues[1] = bodies.get_posX(i);

        if (bodies.get_posY(i) < frontierValues[2])
            frontierValues[2] = bodies.get_posY(i);
        else if (bodies.get_posY(i) > frontierValues[3])
            frontierValues[3] = bodies.get_posY(i);

        if (bodies.get_posZ(i) < frontierValues[4])
            frontierValues[4] = bodies.get_posZ(i);
        else if (bodies.get_posZ(i) > frontierValues[5])
            frontierValues[5] = bodies.get_posZ(i);
    }
    double exactSize = std::max(std::max(frontierValues[1] - frontierValues[0],
        frontierValues[3] - frontierValues[2]),
            frontierValues[5] - frontierValues[4]);
    double nearestPow2 = log2(exactSize);
    int minPow2 = (int)nearestPow2;
    int maxPow2 = minPow2 + 1;

    root->set_size(pow(2, maxPow2));
    // std::cout << "tamanio espacial exacto " << exactSize << std::endl;
    // std::cout << "tamanio espacial escalado "  << root->get_size()<<std::endl;
    root->set_center(
        Vec3(
        (frontierValues[0] + frontierValues[1])/2,
        (frontierValues[2] + frontierValues[3])/2,
        (frontierValues[4] + frontierValues[5])/2
        )
        );
}
