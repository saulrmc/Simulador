//
// Created by Saúl on 15/02/2026.
//

#ifndef SIMULADORGRAVITACIONAL_CREATION_H
#define SIMULADORGRAVITACIONAL_CREATION_H
#include <vector>
#include "../Commons/CelestialBody.h"

class Creation {
    public:
    Creation();
    virtual ~Creation();

    void create_planet(const std::string&, const Vec3&, const Vec3&, const double, const double);
    void create_star(const std::string&, const Vec3&, const Vec3&, const double, const double);
    void create_brown_dwarf(const std::string&, const Vec3&, const Vec3&, const double, const double);
    void create_body(const std::string&, const Vec3&, const Vec3&, const double, const double);
    double get_jupiter_mass() const;


    //funciones para crear cuerpos conocidos y evitar tener que setear sus valores "a mano"
    void create_earth(Vec3 &position, Vec3 &velocity);
    void create_sun(Vec3 &position, Vec3 &velocity);
    void create_moon(Vec3 &position, Vec3 &velocity);
    void create_solar_system(Vec3 &position, Vec3 &velocity);

    private:
    std::vector<CelestialBody*> bodies{};
    double jupiter_mass; //para distinguir planetas de estrellas
};


#endif //SIMULADORGRAVITACIONAL_CREATION_H