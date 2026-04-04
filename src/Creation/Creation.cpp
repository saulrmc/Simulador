//
// Created by Saúl on 15/02/2026.
//

#include "Creation.h"


#define JUPITER_MASS  317.8;

void delete_bodies(std::vector<CelestialBody *> &bodies) {
    for (int i = 0; i < bodies.size(); i++) {
        delete bodies[i];
    }
}

void create_planet(std::vector<CelestialBody *> &bodies, const std::string &name,
    const Vec3& position, const Vec3& velocity, const double mass, const double radius) {
    int currentId = 0;
    if (bodies.size() != 0) currentId = bodies.back()->get_id();

    CelestialBody *body=nullptr;
    body = new Planet();
    body->set_name(name);
    body->set_position(position);
    body->set_velocity(velocity);
    body->set_mass(mass);
    body->set_radius(radius);
    body->set_id(currentId + 1);
    body->set_index(static_cast<int>(bodies.size()));
    bodies.push_back(body);
}

void create_star(std::vector<CelestialBody *> &bodies, const std::string &name,
    const Vec3& position, const Vec3& velocity, const double mass, const double radius) {
    int currentId = 0;
    if (bodies.size() != 0) currentId = bodies.back()->get_id();
    CelestialBody *body=nullptr;
    body = new Star();
    body->set_name(name);
    body->set_position(position);
    body->set_velocity(velocity);
    body->set_mass(mass);
    body->set_radius(radius);
    body->set_id(currentId + 1);
    body->set_index(static_cast<int>(bodies.size()));
    bodies.push_back(body);
}

void create_brown_dwarf(std::vector<CelestialBody *> &bodies, const std::string &name,
    const Vec3& position, const Vec3& velocity, const double mass, const double radius) {
    int currentId = 0;
    if (bodies.size() != 0) currentId = bodies.back()->get_id();
    CelestialBody *body=nullptr;
    body = new BrownDwarf();
    body->set_name(name);
    body->set_position(position);
    body->set_velocity(velocity);
    body->set_mass(mass);
    body->set_radius(radius);
    body->set_id(currentId + 1);
    body->set_index(static_cast<int>(bodies.size()));
    bodies.push_back(body);
}

void create_body(std::vector<CelestialBody *> &bodies, const std::string &name,
    const Vec3& position, const Vec3& velocity, const double mass, const double radius) {
    double jm = JUPITER_MASS;
    if (mass <= 13 * jm) {
        //faltaría hacer la distinción entre planetas gaseosos y rocosos en el futuro
        create_planet(bodies, name, position, velocity, mass, radius);
    }
    else if (mass > 13 * jm and mass <= 80 * jm)
        create_brown_dwarf(bodies, name, position, velocity, mass, radius);
    else create_star(bodies, name, position, velocity, mass, radius);
    //todavia no voy a incluir agujeros negros xd
}

void delete_body(std::vector<CelestialBody*> &bodies, const int index_body) {
    CelestialBody *erase = bodies[index_body];
    bodies[index_body] = bodies.back();
    bodies[index_body]->set_index(index_body);
    bodies.pop_back();
    delete erase;
}

//ojo que la complejidad de esto es O(n) pero también se debe considerar que el vector
//no está ordenado por nombre y que además es poco intuitivo buscar por index
void update_body(std::vector<CelestialBody*> &bodies, const std::string &currentName,
    const std::string &newName, const Vec3& newPosition, const Vec3& newVelocity,
    const double newMass, const double newRadius) {
    for (int i = 0; i < bodies.size(); i++) {
        if (bodies[i]->get_name() == currentName) {
            //puede ser que el cuerpo actualizado ya no tenga la masa para un planeta sino
            //que ahora sea una enana marrón por ejemplo
            delete_body(bodies, i);
            create_body(bodies, newName, newPosition, newVelocity, newMass, newRadius);
        }
    }
}

