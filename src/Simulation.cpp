//
// Created by Saúl on 17/01/2026.
//

#include "Simulation.h"

#include <iomanip>
#include <iostream>
Simulation::Simulation() {
    calcManager = CalculationManager();
}

Simulation::~Simulation() {
}

void Simulation::run_simulation() {
    //para probar el correcto funcionamiento...
    double v_circular = sqrt(units::G * (1.0 + 0.0123) / 1.0);
    CelestialBody earth("Earth", Vec3(0, 0, 0), Vec3(0, -0.002, 0), 1, 0.0165);
    CelestialBody moon("Moon", Vec3(1, 0, 0), Vec3(0, v_circular, 0), 0.0123, 0.00452);
    earth.set_velocity(Vec3(0,-v_circular*moon.get_mass()/(earth.get_mass() + moon.get_mass()), 0));
    moon.set_velocity(Vec3(0,v_circular*earth.get_mass()/(earth.get_mass() + moon.get_mass()), 0));
    earth.set_position(Vec3(- moon.get_mass()/(earth.get_mass() + moon.get_mass()), 0, 0));
    moon.set_position(Vec3(earth.get_mass()/(earth.get_mass() + moon.get_mass()), 0, 0));

    // CelestialBody sun("Sun", Vec3(0, 0, 0), Vec3(0,  -2.01e-5, 0), 333030, 1.80983);
    // CelestialBody earth("Earth", Vec3(389.1724, 0, 0), Vec3(0, 6.69802, 0), 1, 0.0165);
    //para probar la colision...
    //CelestialBody earth("Earth", Vec3(0, 0, 0), Vec3(0, 0, 0), 1, 0.0165);
    //CelestialBody moon("Moon", Vec3(1, 0, 0), Vec3(0, 0, 0), 0.0123, 0.00452);

    //sistema Sol-Tierra-Luna
    // double v_circular2 = sqrt(units::G * (333030) / 389.1724);
    // CelestialBody sun("Sun", Vec3(0, 0, 0), Vec3(0,  -2.01e-5, 0), 333030, 1.80983);
    // CelestialBody earth("Earth", Vec3(0, 0, 0), Vec3(0, -0.002, 0), 1, 0.0165);
    // CelestialBody moon("Moon", Vec3(1, 0, 0), Vec3(0, v_circular, 0), 0.0123, 0.00452);
    // sun.set_velocity(Vec3(0, -v_circular2 * (moon.get_mass() + earth.get_mass())/(1.0 + 333030 + 0.0123), 0));
    // earth.set_velocity(Vec3(0,v_circular2*sun.get_mass()/(1.0 + 333030 + 0.0123)-v_circular*moon.get_mass()/(earth.get_mass() + moon.get_mass()), 0));
    // moon.set_velocity(Vec3(0,v_circular2*sun.get_mass()/(1.0 + 333030 + 0.0123) + v_circular*earth.get_mass()/(earth.get_mass() + moon.get_mass()), 0));
    //
    // sun.set_position(Vec3(-389.1724 * (moon.get_mass() + earth.get_mass())/(1.0 + 333030 + 0.0123), 0, 0));
    // earth.set_position(Vec3(389.1724*(sun.get_mass()/(1.0 + 333030 + 0.0123)) - moon.get_mass()/(earth.get_mass() + moon.get_mass()), 0, 0));
    // moon.set_position(Vec3(389.1724*(sun.get_mass()/(1.0 + 333030 + 0.0123)) + earth.get_mass()/(earth.get_mass() + moon.get_mass()), 0, 0));
    std::cout << std::setprecision(10) << std::fixed << "v_circular inicial: "<< v_circular << std::fixed << std::endl;
    bodies.push_back(earth);
    bodies.push_back(moon);
    //bodies.push_back(sun);
    calcManager.update_forces(bodies);
    int i = 0;
    double k_energy=0;
    Vec3 momentum;

    for (CelestialBody &body : bodies) momentum = momentum + body.get_velocity() * body.get_mass();
    std::cout << "Initial momentum: " << "(" << momentum.get_x() << ", " << momentum.get_y() << ", "
    << momentum.get_z() << ")"<<std::endl;
    momentum = Vec3(0, 0, 0);

    while (i<=27000) {
        calcManager.step(bodies);
        if (i%1000==0) {
            std::cout << "==============================" << std::endl;
            for (CelestialBody &body : bodies) {
                std::cout << "Name: " << body.get_name() << std::endl;
                std::cout << "Position: (" << body.get_position().get_x() << ", " <<
                    body.get_position().get_y() << ", " << body.get_position().get_z() <<
                        ")" << std::endl;
                std::cout << "Velocity: (" << body.get_velocity().get_x() << ", " <<
                    body.get_velocity().get_y() << ", " << body.get_velocity().get_z() <<
                        ")" << std::endl;
            }
                //verificar la energía del sistema:
            double g_energy=0;
            for (size_t i_2 = 0; i_2 < bodies.size(); i_2++) {
                for (size_t j = i_2 + 1; j < bodies.size(); j++) {
                    Vec3 r = bodies[j].get_position() - bodies[i_2].get_position();
                    double dist = r.magnitude();
                    // Evitar división por cero
                    if (dist > 1e-10) {
                        g_energy += -units::G * bodies[i_2].get_mass() * bodies[j].get_mass() / dist;
                    }
                }
            }

            for (CelestialBody &body : bodies) {
                k_energy += 0.5*body.get_mass()*body.get_velocity().magnitude()*body.get_velocity().magnitude();
            }
            std::cout << "===>Total energy = " << k_energy + g_energy<< std::endl;
            k_energy = 0;
            std::cout << "==============================" << std::endl;
            for (CelestialBody &body : bodies) momentum = momentum + body.get_velocity() * body.get_mass();
            std::cout << "Momentum: " << "(" << momentum.get_x() << ", " << momentum.get_y() << ", "
            << momentum.get_z() << ")" << std::endl;
            momentum = Vec3(0, 0, 0);

            std::cout << "Num bodies: " << calcManager.get_num_bodies() << std::endl;
            std::cout << "Theta: " << calcManager.get_theta() << std::endl;
            //i=0;
        }
        i++;
    }
    for (CelestialBody &body : bodies) momentum = momentum + body.get_velocity() * body.get_mass();
    std::cout << "Final momentum: " << "(" << momentum.get_x() << ", " << momentum.get_y() << ", "
    << momentum.get_z() << ")"<< std::endl;
}
