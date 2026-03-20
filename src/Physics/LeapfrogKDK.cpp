//
// Created by Saúl on 20/03/2026.
//
#include "LeapfrogKDK.h"

void leapfrog_integration_kick(std::vector<CelestialBody*> &bodies) {
    for (CelestialBody *&body : bodies) {
        Vec3 next_velocity = next_velocity_for_delta_time(units::HALF_DELTA_TIME,
            body->get_mass(), body->get_force(), body->get_velocity());
        body->set_velocity(next_velocity);
    }
}

void leapfrog_integration_drift(std::vector<CelestialBody *> &bodies) {
    for (CelestialBody *&body : bodies) {
        Vec3 next_position = next_position_for_delta_time(units::DELTA_TIME, body->get_velocity(), body->get_position());
        body->set_position(next_position);
    }
}
