//
// Created by Saúl on 15/02/2026.
//

#include "Creation.h"


#define JUPITER_MASS  317.8;

void create_body(CelestialBodies &bodies, const std::string &name,
    const Vec3& position, const Vec3& velocity, const double mass, const double radius) {
    bodies.push_back_name(name);
    bodies.push_back_position(position);
    bodies.push_back_velocity(velocity);
    bodies.push_back_force(Vec3(0, 0, 0));
    bodies.push_back_mass(mass);
    bodies.push_back_radius(radius);
    bodies.push_back_deleted(false);
}

void delete_body(CelestialBodies &bodies, const int index) {
    bodies.set_name(bodies.back_name(), index);
    bodies.set_position(bodies.back_position(), index);
    bodies.set_velocity(bodies.back_velocity(), index);
    bodies.set_force(bodies.back_force(), index);
    bodies.set_mass(bodies.back_mass(), index);
    bodies.set_radius(bodies.back_radius(), index);

    bodies.pop_back_name();
    bodies.pop_back_position();
    bodies.pop_back_velocity();
    bodies.pop_back_force();
    bodies.pop_back_mass();
    bodies.pop_back_radius();
}

void update_body(CelestialBodies &bodies, const std::string &currentName,
    const std::string &newName, const Vec3& newPosition, const Vec3& newVelocity,
    const double newMass, const double newRadius) {
    for (int i = 0; i < bodies.size(); i++) {
        if (bodies.get_name(i) == currentName) {
            //puede ser que el cuerpo actualizado ya no tenga la masa para un planeta sino
            //que ahora sea una enana marrón por ejemplo
            delete_body(bodies, i);
            create_body(bodies, newName, newPosition, newVelocity, newMass, newRadius);
        }
    }
}

