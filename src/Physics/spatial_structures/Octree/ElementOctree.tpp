//
// Created by Saúl on 8/01/2026.
//
#pragma once

#include "ElementOctree.h"

template<typename T>
ElementOctree<T>::ElementOctree() {
    centerOfMassX = 0;
    centerOfMassY = 0;
    centerOfMassZ = 0;
    mass = 0;
    centerX = 0;
    centerY = 0;
    centerZ = 0;
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
double ElementOctree<T>::get_center_x() const {
    return centerX;
}

template<typename T>
void ElementOctree<T>::set_center_x(const double center_x) {
    this->centerX = center_x;
}

template<typename T>
double ElementOctree<T>::get_center_y() const {
    return centerY;
}

template<typename T>
void ElementOctree<T>::set_center_y(const double center_y) {
    this->centerY = center_y;
}

template<typename T>
double ElementOctree<T>::get_center_z() const {
    return centerZ;
}

template<typename T>
void ElementOctree<T>::set_center_z(const double center_z) {
    this->centerZ = center_z;
}

template<typename T>
double ElementOctree<T>::get_center_of_mass_x() const {
    return centerOfMassX;
}

template<typename T>
void ElementOctree<T>::set_center_of_mass_x(const double center_of_mass_x) {
    this->centerOfMassX = center_of_mass_x;
}

template<typename T>
double ElementOctree<T>::get_center_of_mass_y() const {
    return centerOfMassY;
}

template<typename T>
void ElementOctree<T>::set_center_of_mass_y(const double center_of_mass_y) {
    this->centerOfMassY = center_of_mass_y;
}

template<typename T>
double ElementOctree<T>::get_center_of_mass_z() const {
    return centerOfMassZ;
}

template<typename T>
void ElementOctree<T>::set_center_of_mass_z(const double center_of_mass_z) {
    this->centerOfMassZ = center_of_mass_z;
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
void ElementOctree<T>::set_position(double X, double Y, double Z) {
    this->centerX = X;
    this->centerY = Y;
    this->centerZ = Z;
}

template<typename T>
void ElementOctree<T>::set_center_of_mass(double X, double Y, double Z) {
    this->centerOfMassX = X;
    this->centerOfMassY = Y;
    this->centerOfMassZ = Z;
}

template<typename T>
double ElementOctree<T>::get_size() const {
    return size;
}

template<typename T>
void ElementOctree<T>::set_size(const double size) {
    this->size = size;
}

