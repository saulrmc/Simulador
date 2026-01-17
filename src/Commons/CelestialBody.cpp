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
}

CelestialBody::CelestialBody(std::string name, Vec3 position, Vec3 velocity,
    double mass, double radius) {
    this->name = name;
    this->position = position;
    this->velocity = velocity;
    this->mass = mass;
    this->radius = radius;
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

void CelestialBody::minor_collision(const CelestialBody &cuerpo) {
    this->accumulatedMass += cuerpo.mass;
}

bool CelestialBody::is_it_insignificant(const CelestialBody &cuerpo) {
    if (cuerpo.mass + this->mass == this->mass) return true;
    //o podría ser en base a un porcentaje, por ejemplo el 0,0001% de la masa
    return false;
}

