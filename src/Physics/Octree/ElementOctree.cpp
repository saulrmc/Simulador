//
// Created by Saúl on 8/01/2026.
//

#include "ElementOctree.h"

Vec3 ElementOctree::get_center_of_mass() const {
    return centerOfMass;
}

void ElementOctree::set_center_of_mass(const Vec3 &center_of_mass) {
    centerOfMass = center_of_mass;
}

double ElementOctree::get_mass() const {
    return mass;
}

void ElementOctree::set_mass(const double mass) {
    this->mass = mass;
}

Vec3 ElementOctree::get_position() const {
    return position;
}

void ElementOctree::set_position(const Vec3 &position) {
    this->position = position;
}

double ElementOctree::get_size() const {
    return size;
}

void ElementOctree::set_size(const double size) {
    this->size = size;
}
