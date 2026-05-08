//
// Created by Saúl on 8/01/2026.
//

#ifndef SIMULADORGRAVITACIONAL_ELEMENTOCTREE_H
#define SIMULADORGRAVITACIONAL_ELEMENTOCTREE_H

#include "../src/Commons/Vec3.h"
#include <vector>
constexpr int CAPACITY=4;
template<typename T>
class Octree;

template<typename T>
class NodeOctree;

template<typename T>
class ElementOctree {
public:
    ElementOctree();
    virtual ~ElementOctree();

    Vec3 get_center_of_mass() const;
    void set_center_of_mass(const Vec3 &center_of_mass);
    double get_mass() const;
    void set_mass(const double mass);
    Vec3 get_position() const;
    void set_position(const Vec3 &position);
    double get_size() const;
    void set_size(const double size);
    friend class Octree<T>;
    friend class NodeOctree<T>;
    private:
    Vec3 centerOfMass; //centro de masa del cubo
    double mass; //masa total que está contenida en el espacio cúbico
    Vec3 center;  // Centro del cubo
    double size;     // Tamaño de un lado del cubo

    //T* body;  // Cuerpo contenido en este nodo si es un nodo externo (sin hijos)
    std::vector<T*> bodies{};
};

#include "ElementOctree.tpp"
#endif //SIMULADORGRAVITACIONAL_ELEMENTOCTREE_H