//
// Created by Saúl on 15/02/2026.
//

#ifndef SIMULADORGRAVITACIONAL_CREATION_H
#define SIMULADORGRAVITACIONAL_CREATION_H
#include <vector>
#include "../Commons/CelestialBodies.h"

void create_body(CelestialBodies &bodies, const std::string &name,
    const Vec3& position, const Vec3& velocity, const double mass, const double radius);
void delete_body(CelestialBodies &bodies, const int index);
void update_body(CelestialBodies &bodies, const std::string &currentName,
    const std::string &newName, const Vec3& newPosition, const Vec3& newVelocity,
    const double newMass, const double newRadius);

#endif //SIMULADORGRAVITACIONAL_CREATION_H