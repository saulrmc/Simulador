//
// Created by Saúl on 8/01/2026.
//
#ifndef SIMULADORGRAVITACIONAL_OCTREE_H
#define SIMULADORGRAVITACIONAL_OCTREE_H
#include <cstdint>

#include "NodeOctree.h"
#include "../Physics.h"

class Octree {
public:
    //getters temporales (tal vez) para testear
    int get_num_bodies() const;
    double get_theta() const;

    Octree();
    virtual ~Octree();
    void insert(CelestialBody *body);
    void erase(CelestialBody *body);
    void create_space();
    void calc_forces_per_body(CelestialBody *body);
    NodeOctree* locate_node_father(CelestialBody*);
    NodeOctree* locate_body(CelestialBody *);
    void check_collisions(CelestialBody*, Vec3 &next_position);

    private:
    NodeOctree* root;
    NodeOctree* recursively_locate_node_father(NodeOctree* node, CelestialBody* body);
    NodeOctree* select_child(NodeOctree* node, CelestialBody* body);
    NodeOctree* recursively_locate_body(NodeOctree *node, CelestialBody *body);
    void recursively_insert(NodeOctree*&, CelestialBody *);
    bool recursively_erase(NodeOctree*&, CelestialBody *);
    void recursively_calc_forces(const NodeOctree *node, CelestialBody *body);
    uint8_t octant_for_position(const Vec3 &pos, const Vec3& center);
    //por las pruebas que realicé es mejor dejar en 0 el valor de THETA cuando hay pocos cuerpos
    //porque de lo contrario añade error pero tampoco puede quedarse en 0 porque sino no tiene sentido aplicar un
    //octree si es que los cálculos van a ser por fuerza bruta
    int num_bodies;
    double theta = 0;

};


#endif //SIMULADORGRAVITACIONAL_OCTREE_H