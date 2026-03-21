//
// Created by Saúl on 15/02/2026.
//

#ifndef SIMULADORGRAVITACIONAL_CREATION_H
#define SIMULADORGRAVITACIONAL_CREATION_H
#include <vector>
#include "../Commons/CelestialBody.h"
#include "../Commons/BrownDwarf.h"
#include "../Commons/Planet.h"
#include "../Commons/Star.h"

void create_planet(std::vector<CelestialBody*> &bodies, const std::string&,
    const Vec3&, const Vec3&, const double, const double);
void create_star(std::vector<CelestialBody*> &bodies, const std::string&,
    const Vec3&, const Vec3&, const double, const double);
void create_brown_dwarf(std::vector<CelestialBody*> &bodies, const std::string&,
    const Vec3&, const Vec3&, const double, const double);
void create_body(std::vector<CelestialBody*> &bodies, const std::string&,
    const Vec3&, const Vec3&, const double, const double);

//funciones para crear cuerpos conocidos y evitar tener que setear sus valores "a mano"
void create_earth(Vec3 &position, Vec3 &velocity);
void create_sun(Vec3 &position, Vec3 &velocity);
void create_moon(Vec3 &position, Vec3 &velocity);
void create_solar_system(Vec3 &position, Vec3 &velocity);

void delete_body(std::vector<CelestialBody*> &bodies, const int index_body);
void update_body(std::vector<CelestialBody*> &bodies, const std::string &currentName,
    const std::string &newName, const Vec3& newPosition, const Vec3& newVelocity,
    const double newMass, const double newRadius);

#endif //SIMULADORGRAVITACIONAL_CREATION_H