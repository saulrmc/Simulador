//
// Created by Saúl on 20/03/2026.
//
#include "LeapfrogKDK.h"
#include <iostream>
#include <chrono>
void leapfrog_integration_kick(CelestialBodies &bodies) {
    std::chrono::duration<double, std::micro> total_time(0);
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < bodies.size(); i++) {
        Vec3 next_velocity = next_velocity_for_delta_time(units::HALF_DELTA_TIME,
            bodies.get_mass(i), bodies.get_force(i), bodies.get_velocity(i));
        bodies.set_velocity(next_velocity, i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    total_time = end - start;
    std::cout << std::endl << "Tiempo de kick en microsegundos : "
    << total_time.count() << std::endl;
}

void leapfrog_integration_drift(CelestialBodies &bodies) {
    std::chrono::duration<double, std::micro> total_time(0);
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < bodies.size(); i++) {
        Vec3 next_position = next_position_for_delta_time(units::DELTA_TIME, bodies.get_velocity(i), bodies.get_position(i));
        bodies.set_position(next_position, i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    total_time = end - start;
    std::cout << std::endl << "Tiempo de drift en microsegundos : "
    << total_time.count() << std::endl;
}
