//
// Created by Saúl on 15/02/2026.
//

#include "Creation.h"

#include "../Commons/BrownDwarf.h"
#include "../Commons/Planet.h"
#include "../Commons/Star.h"

Creation::Creation() {
    jupiter_mass = units::MASS_0 * 317.8;
}

Creation::~Creation() {
    for (int i = 0; i < bodies.size(); i++) {
        delete bodies[i];
    }
}

void Creation::create_planet(const std::string &name, const Vec3& position, const Vec3& velocity,
    const double mass, const double radius) {
    CelestialBody *body=nullptr;
    body = new Planet();
    body->set_name(name);
    body->set_position(position);
    body->set_velocity(velocity);
    body->set_mass(mass);
    body->set_radius(radius);
    bodies.push_back(body);
}

void Creation::create_star(const std::string &name, const Vec3& position, const Vec3& velocity,
    const double mass, const double radius) {
    CelestialBody *body=nullptr;
    body = new Star();
    body->set_name(name);
    body->set_position(position);
    body->set_velocity(velocity);
    body->set_mass(mass);
    body->set_radius(radius);
    bodies.push_back(body);
}

void Creation::create_brown_dwarf(const std::string &name, const Vec3& position, const Vec3& velocity,
    const double mass, const double radius) {
    CelestialBody *body=nullptr;
    body = new BrownDwarf();
    body->set_name(name);
    body->set_position(position);
    body->set_velocity(velocity);
    body->set_mass(mass);
    body->set_radius(radius);
    bodies.push_back(body);
}

void Creation::create_body(const std::string &name, const Vec3& position, const Vec3& velocity,
    const double mass, const double radius) {
    if (mass <= 13 * jupiter_mass) {
        //faltaría hacer la distinción entre planetas gaseosos y rocosos en el futuro
        create_planet(name, position, velocity, mass, radius);
    }
    else if (mass > 13 * jupiter_mass and mass <= 80 * jupiter_mass)
        create_brown_dwarf(name, position, velocity, mass, radius);
    else create_star(name, position, velocity, mass, radius);
    //todavia no voy a incluir agujeros negros xd
}

double Creation::get_jupiter_mass() const {
    return jupiter_mass;
}


