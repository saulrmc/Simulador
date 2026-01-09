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

    private:
    Vec3 centerOfMass;
    double mass;
    Vec3 position;  // La posición de la región del nodo (el centro de la celda)
    double size;     // El tamaño de la celda representada por este nodo
    CelestialBody* body;  // Cuerpo contenido en este nodo si es una hoja
};


#endif //SIMULADORGRAVITACIONAL_ELEMENTOCTREE_H