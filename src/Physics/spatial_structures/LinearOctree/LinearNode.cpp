//
// Created by saul on 3/05/26.
//

#include "LinearNode.h"

template<typename T>
LinearNode<T>::LinearNode() {
}

template<typename T>
LinearNode<T>::~LinearNode() {
}

template<typename T>
double LinearNode<T>::get_mass() const {
    return mass;
}

template<typename T>
void LinearNode<T>::set_mass(const double mass) {
    this->mass = mass;
}

template<typename T>
double LinearNode<T>::get_centerX() const {
    return center[0];
}

template<typename T>
void LinearNode<T>::set_centerX(const double centerX) {
    this->center[0] = centerX;
}

template<typename T>
double LinearNode<T>::get_centerY() const {
    return center[1];
}

template<typename T>
void LinearNode<T>::set_centerY(const double centerY) {
    this->center[1] = centerY;
}

template<typename T>
double LinearNode<T>::get_centerZ() const {
    return center[2];
}

template<typename T>
void LinearNode<T>::set_centerZ(const double centerZ) {
    this->center[2] = centerZ;
}

template<typename T>
double LinearNode<T>::get_centerOfMassX() const {
    return centerOfMass[0];
}

template<typename T>
void LinearNode<T>::set_centerOfMassX(const double centerOfMassX) {
    this->centerOfMass[0] = centerOfMassX;
}

template<typename T>
double LinearNode<T>::get_centerOfMassY() const {
    return centerOfMass[1];
}

template<typename T>
void LinearNode<T>::set_centerOfMassY(const double centerOfMassY) {
    this->centerOfMass[1] = centerOfMassY;
}

template<typename T>
double LinearNode<T>::get_centerOfMassZ() const {
    return centerOfMass[2];
}

template<typename T>
void LinearNode<T>::set_centerOfMassZ(const double centerOfMassZ) {
    this->centerOfMass[2] = centerOfMassZ;
}

template<typename T>
unsigned int LinearNode<T>::get_minRange() const {
    return minRange;
}

template<typename T>
void LinearNode<T>::set_minRange(const unsigned int minRange) {
    this->minRange = minRange;
}

template<typename T>
unsigned int LinearNode<T>::get_maxRange() const {
    return maxRange;
}

template<typename T>
void LinearNode<T>::set_maxRange(const unsigned int maxRange) {
    this->maxRange = maxRange;
}
