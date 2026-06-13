//
// Created by Saúl on 8/01/2026.
//

#ifndef SIMULADORGRAVITACIONAL_ELEMENTOCTREE_H
#define SIMULADORGRAVITACIONAL_ELEMENTOCTREE_H

#include "Vec3.h"
#include <vector>
constexpr int CAPACITY=4;
class ElementOctree {
public:
    ElementOctree();
    virtual ~ElementOctree();

    Vec3 get_center() const;
    void set_center(const Vec3 &center);
    Vec3 get_center_of_mass() const;
    void set_center_of_mass(const Vec3 &centerOfMass);
    double get_mass() const;
    void set_mass(double mass);
    double get_size() const;
    void set_size(const double size);

    friend class Octree;
    friend class NodeOctree;

    private:
     //centro de masa del cubo
    Vec3 centerOfMass;
    double mass; //masa total que está contenida en el espacio cúbico

    // Centro del cubo
    Vec3 center;
    double size;     // Tamaño de un lado del cubo

    // Índices de cuerpos contenidos en este nodo si es un nodo externo (sin hijos)
    std::vector<int> indexBodies{};
};

inline ElementOctree::ElementOctree() {
    this->centerOfMass=Vec3(0,0,0);
    mass = 0;
    center=Vec3(0,0,0);
    size = 0;
    //body=nullptr;
}

inline ElementOctree::~ElementOctree() {
    //esta clase no "propietaria" de los cuerpos
    indexBodies.clear();
}

inline Vec3 ElementOctree::get_center() const {
    return this->center;
}

inline void ElementOctree::set_center(const Vec3 &center) {
    this->center = center;
}

inline Vec3 ElementOctree::get_center_of_mass() const {
    return this->centerOfMass;
}

inline void ElementOctree::set_center_of_mass(const Vec3& centerOfMass) {
    this->centerOfMass = centerOfMass;
}

inline double ElementOctree::get_mass() const {
    return this->mass;
}


inline void ElementOctree::set_mass(const double mass) {
    this->mass = mass;
}

inline double ElementOctree::get_size() const {
    return size;
}

inline void ElementOctree::set_size(const double size) {
    this->size = size;
}
#endif //SIMULADORGRAVITACIONAL_ELEMENTOCTREE_H