//
// Created by Saúl on 21/12/2025.
//
#pragma once
#ifndef SIMULADORGRAVITACIONAL_CELESTIALBODY_H
#define SIMULADORGRAVITACIONAL_CELESTIALBODY_H
#include <algorithm>
#include <string>
#include <vector>
#include "Vec3.h"

class CelestialBodies {
    public:
    CelestialBodies();
    virtual ~CelestialBodies();

    std::string get_name(int index) const;
    void set_name(const std::string &name, int index);
    Vec3 get_velocity(int index) const;
    void set_velocity(const Vec3 &velocity, int index);
    Vec3 get_position(int index) const;
    void set_position(const Vec3 &position, int index);
    Vec3 get_force(int index) const;
    void set_force(const Vec3 &force, int index);
    double get_mass(int index) const;
    void set_mass(const double mass, int index);
    double get_radius(int index) const;
    void set_radius(const double radius, int index);

    int get_index() const;
    void set_index(const int index);
    double get_posX(int index) const;
    void set_posX(const double x, int index);
    double get_posY(int index) const;
    void set_posY(const double y, int index);
    double get_posZ(int index) const;
    void set_posZ(const double z, int index);

    void push_back_name(const std::string &name);
    void push_back_velocity(const Vec3 &velocity);
    void push_back_position(const Vec3 &position);
    void push_back_force(const Vec3 &force);
    void push_back_mass(const double mass);
    void push_back_radius(const double radius);
    void push_back_deleted(const bool deleted);
    void pop_back_name();
    void pop_back_velocity();
    void pop_back_position();
    void pop_back_force();
    void pop_back_mass();
    void pop_back_radius();
    void pop_back_deleted();

    bool is_deleted(int index) const;
    void mark_deleted(int index);
    void clear_deleted();
    int apply_deletions();

    std::string back_name() const;
    Vec3 back_position() const;
    Vec3 back_velocity() const;
    Vec3 back_force() const;
    double back_mass() const;
    double back_radius() const;

    unsigned long int size() const;

    void operator=(int index);
    bool operator==(int index) const;

    private:
    std::vector<std::string> names{};
    std::vector<Vec3> positions{};
    std::vector<Vec3> velocities{};
    std::vector<Vec3> forces{};
    std::vector<double> masses{};
    std::vector<double> radii{};
    std::vector<bool> deleted{};
};


inline CelestialBodies::CelestialBodies() {
}

inline CelestialBodies::~CelestialBodies() {
}

inline std::string CelestialBodies::get_name(int index) const {
    return names[index];
}

inline void CelestialBodies::set_name(const std::string &name, int index) {
    this->names[index] = name;
}

inline Vec3 CelestialBodies::get_velocity(int index) const {
    return velocities[index];
}

inline void CelestialBodies::set_velocity(const Vec3 &velocity, int index) {
    this->velocities[index] = velocity;
}

inline Vec3 CelestialBodies::get_position(int index) const {
    return positions[index];
}

inline void CelestialBodies::set_position(const Vec3 &position, int index) {
    this->positions[index] = position;
}

inline Vec3 CelestialBodies::get_force(int index) const {
    return forces[index];
}

inline void CelestialBodies::set_force(const Vec3 &force, int index) {
    this->forces[index] = force;
}

inline double CelestialBodies::get_mass(int index) const {
    return masses[index];
}

inline void CelestialBodies::set_mass(const double mass, int index) {
    this->masses[index] = mass;
}

inline double CelestialBodies::get_radius(int index) const {
    return this->radii[index];

}

inline void CelestialBodies::set_radius(const double radius, int index) {
    this->radii[index] = radius;
}

inline int CelestialBodies::get_index() const {
    return 0;
}

inline void CelestialBodies::set_index(const int index) {
}

inline double CelestialBodies::get_posX(int index) const {
    return positions[index].get_x();
}

inline void CelestialBodies::set_posX(const double x, int index) {
    this->positions[index].set_x(x);
}

inline double CelestialBodies::get_posY(int index) const {
    return positions[index].get_y();
}

inline void CelestialBodies::set_posY(const double y, int index) {
    this->positions[index].set_y(y);
}

inline double CelestialBodies::get_posZ(int index) const {
    return positions[index].get_z();
}

inline void CelestialBodies::set_posZ(const double z, int index) {
    this->positions[index].set_z(z);
}

inline void CelestialBodies::push_back_name(const std::string &name) {
    this->names.push_back(name);
}

inline void CelestialBodies::push_back_velocity(const Vec3 &velocity) {
    this->velocities.push_back(velocity);
}

inline void CelestialBodies::push_back_position(const Vec3 &position) {
    this->positions.push_back(position);
}

inline void CelestialBodies::push_back_force(const Vec3 &force) {
    this->forces.push_back(force);
}

inline void CelestialBodies::push_back_mass(const double mass) {
    this->masses.push_back(mass);
}

inline void CelestialBodies::push_back_radius(const double radius) {
    this->radii.push_back(radius);
}

inline void CelestialBodies::pop_back_name() {
    this->names.pop_back();
}

inline void CelestialBodies::pop_back_velocity() {
    this->velocities.pop_back();
}

inline void CelestialBodies::pop_back_position() {
    this->positions.pop_back();
}

inline void CelestialBodies::pop_back_force() {
    this->forces.pop_back();
}

inline void CelestialBodies::pop_back_mass() {
    this->masses.pop_back();
}

inline void CelestialBodies::pop_back_radius() {
    this->radii.pop_back();
}

inline void CelestialBodies::push_back_deleted(const bool isDeleted) {
    this->deleted.push_back(isDeleted);
}

inline void CelestialBodies::pop_back_deleted() {
    this->deleted.pop_back();
}

inline bool CelestialBodies::is_deleted(int index) const {
    return this->deleted[index];
}

inline void CelestialBodies::mark_deleted(int index) {
    this->deleted[index] = true;
}

inline void CelestialBodies::clear_deleted() {
    std::fill(this->deleted.begin(), this->deleted.end(), false);
}

inline int CelestialBodies::apply_deletions() {
    std::vector<int> toDelete;
    for (int i = 0; i < (int)this->deleted.size(); i++) {
        if (this->deleted[i]) toDelete.push_back(i);
    }
    std::sort(toDelete.begin(), toDelete.end(), std::greater<int>());
    for (int idx : toDelete) {
        int last = (int)this->names.size() - 1;
        if (idx != last) {
            this->names[idx] = this->names[last];
            this->positions[idx] = this->positions[last];
            this->velocities[idx] = this->velocities[last];
            this->forces[idx] = this->forces[last];
            this->masses[idx] = this->masses[last];
            this->radii[idx] = this->radii[last];
            this->deleted[idx] = this->deleted[last];
        }
        this->names.pop_back();
        this->positions.pop_back();
        this->velocities.pop_back();
        this->forces.pop_back();
        this->masses.pop_back();
        this->radii.pop_back();
        this->deleted.pop_back();
    }
    return (int)toDelete.size();
}

inline std::string CelestialBodies::back_name() const {
    return this->names.back();
}

inline Vec3 CelestialBodies::back_position() const {
    return this->positions.back();
}

inline Vec3 CelestialBodies::back_velocity() const {
    return this->velocities.back();
}

inline Vec3 CelestialBodies::back_force() const {
    return this->forces.back();
}

inline double CelestialBodies::back_mass() const {
    return this->masses.back();
}

inline double CelestialBodies::back_radius() const {
    return this->radii.back();
}

inline unsigned long int CelestialBodies::size() const {
    if (
        this->names.size() == this->positions.size()
        and this->positions.size() == this->velocities.size()
        and this->velocities.size() == this->forces.size()
        and this->forces.size() == this->masses.size()
        and this->masses.size() == this->radii.size()
        and this->radii.size() == this->deleted.size()
        ) return this->names.size();
    return 0;
}


#endif //SIMULADORGRAVITACIONAL_CELESTIALBODY_H
