//
// Created by Saúl on 17/01/2026.
//

#include "Simulation.h"

#include <chrono>
#include <iostream>
Simulation::Simulation() {
}

Simulation::~Simulation() {
}

void Simulation::run_simulation() {
    //para probar el correcto funcionamiento...
    // double v_circular = sqrt(units::G * (1.0 + 0.0123) / 1.0);
    // Vec3 earthVel = Vec3(0, -v_circular*0.0123/(1+0.0123), 0);
    // Vec3 earthPos = Vec3(-0.0123/(1+0.0123), 0, 0);
    // Vec3 moonVel = Vec3(0, v_circular*1/(1+0.0123), 0);
    // Vec3 moonPos = Vec3(1/(1+0.0123), 0, 0);
    // create_body(bodies, "Earth", earthPos, earthVel, 1, 0.0165);
    // create_body(bodies, "Moon", moonPos, moonVel, 0.0123, 0.00452);
    int nbodies;
    std::cout << "cantidad de cuerpos en la simulacion:" << std::endl;
    std::cin >> nbodies;
    create_many_bodies(bodies, nbodies);


    // create_body(bodies, "M1", Vec3(-0.034/2 + 0.0123, 0, 0), Vec3(0, 0, 0), 1, 0.0165);
    // create_body(bodies, "M2", Vec3(0, 0.0165*2, 0), Vec3(0, 0, 0), 0.0123, 0.00452);

    // create_body(bodies, "M1", Vec3(-0.034/2, 0, 0), Vec3(0, 0, 0), 1, 0.0165);
    // create_body(bodies, "M2", Vec3(0, 0.02, 0), Vec3(-3.5, 0, 0), 0.0123 + 0.3, 0.00452);

    // earth.set_velocity(Vec30,-v(_circular*moon.get_mass()/(earth.get_mass() + moon.get_mass()), 0));
    // moon.set_velocity(Vec3(0,v_circular*earth.get_mass()/(earth.get_mass() + moon.get_mass()), 0));
    // earth.set_position(Vec3(- moon.get_mass()/(earth.get_mass() + moon.get_mass()), 0, 0));
    // moon.set_position(Vec3(earth.get_mass()/(earth.get_mass() + moon.get_mass()), 0, 0));


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
    //bodies.push_back(sun);

    //algoritmo por fuerza bruta para hallar al cuerpo que esté
    //en el límite inferior de los tres ejes.
    double xMin = 10000, yMin = 10000, zMin = 10000;
    std::string nombre{};
    for (int i = 0; i < nbodies; i++) {
        if (    bodies.get_posX(i) < xMin
            and bodies.get_posY(i) < yMin
            and bodies.get_posZ(i) < zMin
        ) {
            xMin = bodies.get_posX(i);
            yMin = bodies.get_posY(i);
            zMin = bodies.get_posZ(i);
            nombre = bodies.get_name(i);
        }
    }
    std::cout << "objeto en el límite inferior: " <<
        std::endl <<"nombre: " << nombre <<
        std::endl << "X: " << xMin <<
        std::endl << "Y: " << yMin <<
        std::endl << "Z: " << zMin << std::endl;

    calcManager.update_forces(bodies);
    int i = 0;
    double k_energy=0;
    Vec3 momentum;

    for (int j = 0; j < nbodies; j++) momentum = momentum + bodies.get_velocity(j) * bodies.get_mass(j);
    std::cout << "Initial momentum: " << "(" << momentum.get_x() << ", " << momentum.get_y() << ", "
    << momentum.get_z() << ")"<<std::endl;
    momentum = Vec3(0, 0, 0);
    std::cout << "tamanio sizeof(CelestialBody): " << sizeof(CelestialBodies)<<std::endl;
    std::chrono::duration<double, std::micro> step_time(0);
    std::chrono::duration<double> total_time0(0);
    auto startTotal0 = std::chrono::high_resolution_clock::now();
    //while (i<=1) {
        auto start = std::chrono::high_resolution_clock::now();
        calcManager.step(bodies);
        // if (i%1000==0) {
        //       std::cout << "==============================" << std::endl;
        //       for (CelestialBody *&body : bodies) {
        //           std::cout << "Name: " << body->get_name() << std::endl;
        //           std::cout << "Position: (" << body->get_position().get_x() << ", " <<
        //               body->get_position().get_y() << ", " << body->get_position().get_z() <<
        //                   ")" << std::endl;
        //           std::cout << "Velocity: (" << body->get_velocity().get_x() << ", " <<
        //               body->get_velocity().get_y() << ", " << body->get_velocity().get_z() <<
        //                   ")" << std::endl;
        //           std::cout << "Mass:  " << body->get_mass() << std::endl;
        //           std::cout << "Radius: " << body->get_radius() << std::endl;
        //       }
        //           //verificar la energía del sistema:
        //       double g_energy=0;
        //       for (size_t i_2 = 0; i_2 < bodies.size(); i_2++) {
        //           for (size_t j = i_2 + 1; j < bodies.size(); j++) {
        //               Vec3 r = bodies[j]->get_position() - bodies[i_2]->get_position();
        //               double dist = r.magnitude();
        //               // Evitar división por cero
        //               if (dist > 1e-10) {
        //                   g_energy += -units::G * bodies[i_2]->get_mass() * bodies[j]->get_mass() / dist;
        //               }
        //           }
        //       }
        //
        //       for (CelestialBody *&body : bodies) {
        //           k_energy += 0.5*body->get_mass()*body->get_velocity().magnitude()*body->get_velocity().magnitude();
        //       }
        //       std::cout << "===>Mechanical energy (K + U) = " << k_energy + g_energy<< std::endl;
        //       k_energy = 0;
        //       std::cout << "==============================" << std::endl;
        //       for (CelestialBody *&body : bodies) momentum = momentum + body->get_velocity() * body->get_mass();
        //       std::cout << "Momentum: " << "(" << momentum.get_x() << ", " << momentum.get_y() << ", "
        //       << momentum.get_z() << ")" << std::endl;
        //       momentum = Vec3(0, 0, 0);
        //     i=0;
        // }
        i++;
        auto end = std::chrono::high_resolution_clock::now();
        step_time += end - start;
    //}
    auto endTotal0 = std::chrono::high_resolution_clock::now();
    total_time0 = endTotal0 - startTotal0;
    for (int j = 0; j < bodies.size(); j++) momentum = momentum + bodies.get_velocity(j) * bodies.get_mass(j);
    std::cout << "Final momentum: " << "(" << momentum.get_x() << ", " << momentum.get_y() << ", "
    << momentum.get_z() << ")"<< std::endl;
    // std::cout << "visits = " <<visited_nodes << std::endl;
    // std::cout << "leafs   = " << leaf_nodes << '\n';
    // std::cout << "calls  = " << condition_calls << '\n';
    // std::cout << "passed = " << condition_passed << '\n';
    // std::cout << "body_checks = " << body_checks << '\n';
    // std::cout << "total nodes = " << total_nodes<< std::endl;
    std::cout << "AVERAGE TIME (microseconds): " << step_time.count()/i << std::endl;
    std::cout << "TOTAL TIME (seconds): " << total_time0.count() << std::endl;
}

void create_many_bodies(CelestialBodies & bodies, int N) {
    double centerMass = 50.0;
    double radiusMin = 5.0;
    double radiusMax = 1024.0;


    create_body(bodies, "Center", Vec3(0,0,0), Vec3(0,0,0), centerMass, 0.5);

    for (int i = 0; i < N; i++) {
        double t = (double)i / N;


        double r = radiusMin + (radiusMax - radiusMin) * t;


        double angle = ((double)rand() / RAND_MAX) * 2.0 * M_PI;
        //por alguna razón tarda más en ejecutarse si calcula esto:
        // double angle2 = ((double)rand() / RAND_MAX) * 2.0 * M_PI;
        // y genera una posición así:
        // Vec3 pos(
        //     r * cos(angle) * sin(angle2),
        //     r * sin(angle) * sin(angle2),
        //     r * cos(angle2)
        // );

        Vec3 pos(
            r * cos(angle) ,
            r * sin(angle) ,
            r * cos(angle)
        );


        double v = sqrt(units::G * centerMass / r);

        Vec3 vel(
            -v * sin(angle),
             v * cos(angle),
             0
        );


        double mass = 0.001 + ((double)rand() / RAND_MAX) * 0.01;


        double radius = 0.001;

        create_body(bodies, "Body_" + std::to_string(i), pos, vel, mass, radius);
    }
}