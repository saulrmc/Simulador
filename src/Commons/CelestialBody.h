//
// Created by Saúl on 21/12/2025.
//
#pragma once
#ifndef SIMULADORGRAVITACIONAL_CELESTIALBODY_H
#define SIMULADORGRAVITACIONAL_CELESTIALBODY_H
#include <string>
#include "Vec3.h"
#include "../Physics/PhysicsUtils.h"

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
    int get_id() const;
    void set_id(const int id);
    int get_index() const;
    void set_index(const int index);
    double get_posX() const;
    void set_posX(const double x);
    double get_posY() const;
    void set_posY(const double y);
    double get_posZ() const;
    void set_posZ(const double z);


    void operator=(const CelestialBody &);
    bool operator==(const CelestialBody &) const;

    //para el caso de las colisiones
    void minor_collision(const double);
    bool is_it_insignificant(const double);
    virtual double gravitational_biding_energy() = 0;

    private:
    std::string name;
    Vec3 velocity, position, force;
    double mass;
    double radius;
    int id;
    int index;//este atributo es la posición en la estructura de datos que se encuentra
    //por ahora es solo útil por la existencia del vector

    //pero qué ocurre cuando un objeto enorme colisiona con uno muy pequeño?
    //como en el caso del Sol con la Luna.
    //En la vida real una colisión así mantendría al Sol practicamente igual
    //pero ahora qué pasaría si lanzamos un millón de Lunas al Sol una por una?
    //individualmente no hacen mucho pero llega un momento en el que ya no se puede
    //despreciar, por ejemplo, sus masas.
    double accumulatedVel;
    double accumulatedMass;
    //para evitar que los fragmentos también se sigan dividiendo en muchísimos más
};

inline CelestialBody::CelestialBody() {
    name = "CelestialBody";
    velocity = Vec3(0.0f, 0.0f, 0.0f);
    position = Vec3(0.0f, 0.0f, 0.0f);
    force = Vec3(0.0f, 0.0f, 0.0f);
    mass = 0.0f;
    radius = 0.0f;
    id = 0;
}

inline CelestialBody::CelestialBody(const std::string &name, const Vec3& position, const Vec3& velocity,
    const double mass, const double radius) {
    this->name = name;
    this->position = position;
    this->velocity = velocity;
    this->mass = mass;
    this->radius = radius;
}

inline CelestialBody::~CelestialBody() {
}

inline std::string CelestialBody::get_name() const {
    return name;
}

inline void CelestialBody::set_name(const std::string &name) {
    this->name = name;
}

inline Vec3 CelestialBody::get_velocity() const {
    return velocity;
}

inline void CelestialBody::set_velocity(const Vec3 &velocity) {
    this->velocity = velocity;
}

inline Vec3 CelestialBody::get_position() const {
    return position;
}

inline void CelestialBody::set_position(const Vec3 &position) {
    this->position = position;
}

inline Vec3 CelestialBody::get_force() const {
    return force;
}

inline void CelestialBody::set_force(const Vec3 &force) {
    this->force = force;
}

inline double CelestialBody::get_mass() const {
    return mass;
}

inline void CelestialBody::set_mass(const double mass) {
    this->mass = mass;
}

inline double CelestialBody::get_radius() const {
    return radius;
}

inline void CelestialBody::set_radius(const double radio) {
    this->radius = radio;
}

inline void CelestialBody::operator=(const CelestialBody &body) {
    this->name = body.get_name();
    this->position = body.get_position();
    this->velocity = body.get_velocity();
    this->force = body.get_force();
    this->mass = body.get_mass();
    this->radius = body.get_radius();

    this->accumulatedMass = body.accumulatedMass;
    this->accumulatedVel = body.accumulatedVel;
}

inline bool CelestialBody::operator==(const CelestialBody &body) const {
    if (this->name == body.get_name()) return true; //esta es una posible condicion, podría ser permanente
    //o podría cambiarla por otra, probablemente sea mejor crearlas bajo un ID...
    return false;
}

inline void CelestialBody::minor_collision(const double body_mass) {
    this->accumulatedMass += body_mass;
}

inline bool CelestialBody::is_it_insignificant(const double body_mass) {
    if (100*body_mass/this->mass < 0.000001) return true;
    //o podría ser en base a un porcentaje, por ejemplo el 0,0001% de la masa
    return false;
}

inline int CelestialBody::get_id() const {
    return id;
}

inline void CelestialBody::set_id(const int id) {
    this->id = id;
}

inline int CelestialBody::get_index() const {
    return index;
}

inline void CelestialBody::set_index(const int index) {
    this->index = index;
}

inline double CelestialBody::get_posX() const {
    return position.get_x();
}

inline void CelestialBody::set_posX(const double x) {
    position.set_x(x);
}

inline double CelestialBody::get_posY() const {
    return position.get_y();
}

inline void CelestialBody::set_posY(const double y) {
    position.set_y(y);
}

inline double CelestialBody::get_posZ() const {
    return position.get_z();
}

inline void CelestialBody::set_posZ(const double z) {
    position.set_z(z);
}

#endif //SIMULADORGRAVITACIONAL_CELESTIALBODY_H