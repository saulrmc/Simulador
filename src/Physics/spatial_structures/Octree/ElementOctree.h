//
// Created by Saúl on 8/01/2026.
//

#ifndef SIMULADORGRAVITACIONAL_ELEMENTOCTREE_H
#define SIMULADORGRAVITACIONAL_ELEMENTOCTREE_H

#include "Vec3.h"
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

    double get_center_x() const;
    void set_center_x(const double center_x);
    double get_center_y() const;
    void set_center_y(const double center_y);
    double get_center_z() const;
    void set_center_z(const double center_z);
    double get_center_of_mass_x() const;
    void set_center_of_mass_x(const double center_of_mass_x);
    double get_center_of_mass_y() const;
    void set_center_of_mass_y(const double center_of_mass_y);
    double get_center_of_mass_z() const;
    void set_center_of_mass_z(const double center_of_mass_z);
    double get_mass() const;
    void set_mass(const double mass);
    void set_position(double X, double Y, double Z);
    void set_center_of_mass(double X, double Y, double Z);
    double get_size() const;
    void set_size(const double size);
    friend class Octree<T>;
    friend class NodeOctree<T>;
    private:
     //centro de masa del cubo
    double centerOfMassX;
    double centerOfMassY;
    double centerOfMassZ;
    double mass; //masa total que está contenida en el espacio cúbico

    // Centro del cubo
    double centerX;
    double centerY;
    double centerZ;
    double size;     // Tamaño de un lado del cubo

    //T* body;  // Cuerpo contenido en este nodo si es un nodo externo (sin hijos)
    std::vector<T*> bodies{};
};

#include "ElementOctree.tpp"
#endif //SIMULADORGRAVITACIONAL_ELEMENTOCTREE_H