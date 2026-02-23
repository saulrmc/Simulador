//
// Created by Saúl on 21/12/2025.
//

#include "../Commons/CelestialBody.h"

CelestialBody::CelestialBody() {
    name = "CelestialBody";
    velocity = Vec3(0.0f, 0.0f, 0.0f);
    position = Vec3(0.0f, 0.0f, 0.0f);
    force = Vec3(0.0f, 0.0f, 0.0f);
    mass = 0.0f;
    radius = 0.0f;
    isDivisible = true;
}

CelestialBody::CelestialBody(const std::string &name, const Vec3& position, const Vec3& velocity,
    const double mass, const double radius) {
    this->name = name;
    this->position = position;
    this->velocity = velocity;
    this->mass = mass;
    this->radius = radius;
    this->isDivisible = true;
}

CelestialBody::~CelestialBody() {
}

std::string CelestialBody::get_name() const {
    return name;
}

void CelestialBody::set_name(const std::string &name) {
    this->name = name;
}

Vec3 CelestialBody::get_velocity() const {
    return velocity;
}

void CelestialBody::set_velocity(const Vec3 &velocity) {
    this->velocity = velocity;
}

Vec3 CelestialBody::get_position() const {
    return position;
}

void CelestialBody::set_position(const Vec3 &position) {
    this->position = position;
}

Vec3 CelestialBody::get_force() const {
    return force;
}

void CelestialBody::set_force(const Vec3 &force) {
    this->force = force;
}

double CelestialBody::get_mass() const {
    return mass;
}

void CelestialBody::set_mass(const double mass) {
    this->mass = mass;
}

double CelestialBody::get_radius() const {
    return radius;
}

void CelestialBody::set_radius(const double radio) {
    this->radius = radio;
}

void CelestialBody::operator=(const CelestialBody &body) {
    this->name = body.get_name();
    this->position = body.get_position();
    this->velocity = body.get_velocity();
    this->force = body.get_force();
    this->mass = body.get_mass();
    this->radius = body.get_radius();

    this->accumulatedMass = body.accumulatedMass;
    this->accumulatedVel = body.accumulatedVel;
}

bool CelestialBody::operator==(const CelestialBody &body) const {
    if (this->name == body.get_name()) return true; //esta es una posible condicion, podría ser permanente
    //o podría cambiarla por otra, probablemente sea mejor crearlas bajo un ID...
    return false;
}

void CelestialBody::minor_collision(const double body_mass) {
    this->accumulatedMass += body_mass;
}

bool CelestialBody::is_it_insignificant(const double body_mass) {
    if (100*body_mass/this->mass < 0.000001) return true;
    //o podría ser en base a un porcentaje, por ejemplo el 0,0001% de la masa
    return false;
}

bool CelestialBody::is_is_divisible() const {
    return isDivisible;
}

void CelestialBody::set_is_divisible(const bool is_divisible) {
    isDivisible = is_divisible;
}

