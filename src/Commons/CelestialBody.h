//
// Created by Saúl on 21/12/2025.
//
#pragma once
#ifndef SIMULADORGRAVITACIONAL_CELESTIALBODY_H
#define SIMULADORGRAVITACIONAL_CELESTIALBODY_H
#include <string>
#include "../Commons/Vec3.h"

class CelestialBody {
    public:
    CelestialBody();
    CelestialBody(const std::string&, const Vec3&, const Vec3&, const double, const double);
    virtual ~CelestialBody();

    std::string get_name() const;
    void set_name(const std::string &name);
    Vec3 get_velocity() const;
    void set_velocity(const Vec3 &velocity);
    Vec3 get_position() const;
    void set_position(const Vec3 &position);
    Vec3 get_force() const;
    void set_force(const Vec3 &force);
    double get_mass() const;
    void set_mass(const double mass);
    double get_radius() const;
    void set_radius(const double radio);

    void operator=(const CelestialBody &);
    bool operator==(const CelestialBody &) const;

    //para el caso de las colisiones
    void minor_collision(const CelestialBody &);
    bool is_it_insignificant(const CelestialBody &);

    private:
    std::string name;
    Vec3 velocity, position, force;
    double mass;
    double radius;

    //pero qué ocurre cuando un objeto enorme colisiona con uno muy pequeño?
    //como en el caso del Sol con la Luna.
    //En la vida real una colisión así mantendría al Sol practicamente igual
    //pero ahora qué pasaría si lanzamos un millón de Lunas al Sol una por una?
    //individualmente no hacen mucho pero llega un momento en el que ya no se puede
    //despreciar, por ejemplo, sus masas.
    double accumulatedVel;
    double accumulatedMass;
};

#endif //SIMULADORGRAVITACIONAL_CELESTIALBODY_H