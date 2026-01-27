//
// Created by Saúl on 17/01/2026.
//

#include "Simulation.h"
#include <iostream>
Simulation::Simulation() {
    calcManager = CalculationManager();
}

Simulation::~Simulation() {
}

void Simulation::run_simulation() {
    CelestialBody earth("Earth", Vec3(0, 0, 0), Vec3(0, -0.00282, 0), 1, 0.0165);
    CelestialBody moon("Moon", Vec3(1, 0, 0), Vec3(0, 0.22, 0), 0.0123, 0.00452);
    bodies.push_back(earth);
    bodies.push_back(moon);
    calcManager.update_forces(bodies);
    //para probar el correcto funcionamiento la prueba va a consistir en agregar
    //un objeto (no input) y mostrar sus atributos
    //luego agregar otro objeto y ver la interacción
    int i = 0;
    double k_energy=0;
    while (true) {
        calcManager.step(bodies);
        if (i==500000) {
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
            Vec3 r = moon.get_position() - earth.get_position();
            double dist = r.magnitude();
            double g_energy = - units::G * earth.get_mass() * moon.get_mass() / dist;

            for (CelestialBody &body : bodies) {
                k_energy += 0.5*body.get_mass()*body.get_velocity().magnitude()*body.get_velocity().magnitude();
            }
            std::cout << "===>Total energy= " << k_energy + g_energy<< std::endl;
            k_energy = 0;
            std::cout << "==============================" << std::endl;
            i=0;
        }
        i++;
    }
}
