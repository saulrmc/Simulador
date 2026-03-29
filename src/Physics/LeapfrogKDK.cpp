//
// Created by Saúl on 20/03/2026.
//
#include "LeapfrogKDK.h"
#include <iostream>
#include <chrono>
void leapfrog_integration_kick(std::vector<CelestialBody*> &bodies) {
    // std::chrono::duration<double, std::micro> total_time(0);
    // auto start = std::chrono::high_resolution_clock::now();
    for (CelestialBody *&body : bodies) {
        Vec3 next_velocity = next_velocity_for_delta_time(units::HALF_DELTA_TIME,
            body->get_mass(), body->get_force(), body->get_velocity());
        body->set_velocity(next_velocity);
    }
    // auto end = std::chrono::high_resolution_clock::now();
    // total_time = end - start;
    // std::cout << std::endl << "Tiempo de kick en microsegundos : "
    // << total_time.count() << std::endl;
}

void leapfrog_integration_drift(std::vector<CelestialBody *> &bodies) {
    // std::chrono::duration<double, std::micro> total_time(0);
    // auto start = std::chrono::high_resolution_clock::now();
    for (CelestialBody *&body : bodies) {
        Vec3 next_position = next_position_for_delta_time(units::DELTA_TIME, body->get_velocity(), body->get_position());
        body->set_position(next_position);
    }
    // auto end = std::chrono::high_resolution_clock::now();
    // total_time = end - start;
    // std::cout << std::endl << "Tiempo de drift en microsegundos : "
    // << total_time.count() << std::endl;
}
