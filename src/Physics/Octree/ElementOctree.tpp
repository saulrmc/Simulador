//
// Created by Saúl on 8/01/2026.
//

#include "ElementOctree.h"

template<typename T>
ElementOctree<T>::ElementOctree() {
    centerOfMass = Vec3(0, 0, 0);
    mass = 0;
    center = Vec3(0, 0, 0);
    size = 0;
    //body=nullptr;
}

template<typename T>
ElementOctree<T>::~ElementOctree() {
    //los punteros T* body NO deben borrarse porque
    //esta clase no "propietaria" de los cuerpos
    bodies.clear();
}

template<typename T>
Vec3 ElementOctree<T>::get_center_of_mass() const {
    return centerOfMass;
}

template<typename T>
void ElementOctree<T>::set_center_of_mass(const Vec3 &center_of_mass) {
    centerOfMass = center_of_mass;
}

template<typename T>
double ElementOctree<T>::get_mass() const {
    return mass;
}

template<typename T>
void ElementOctree<T>::set_mass(const double mass) {
    this->mass = mass;
}

template<typename T>
Vec3 ElementOctree<T>::get_position() const {
    return center;
}

template<typename T>
void ElementOctree<T>::set_position(const Vec3 &position) {
    this->center = position;
}

template<typename T>
double ElementOctree<T>::get_size() const {
    return size;
}

template<typename T>
void ElementOctree<T>::set_size(const double size) {
    this->size = size;
}

