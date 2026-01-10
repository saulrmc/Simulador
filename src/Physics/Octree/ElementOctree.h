//
// Created by Saúl on 8/01/2026.
//

#ifndef SIMULADORGRAVITACIONAL_ELEMENTOCTREE_H
#define SIMULADORGRAVITACIONAL_ELEMENTOCTREE_H
#include "../src/Creation/CelestialBody.h"
#include "../src/Creation/Vec3.h"

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
    friend class Octree;
    friend class NodeOctree;
    private:
    Vec3 centerOfMass; //centro de masa del cubo
    double mass; //
    Vec3 position;  // Centro del cubo
    double size;     // Tamaño de un lado del cubo
    CelestialBody* body;  // Cuerpo contenido en este nodo si es un nodo externo (sin hijos)
};


#endif //SIMULADORGRAVITACIONAL_ELEMENTOCTREE_H