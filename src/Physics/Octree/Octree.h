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
    Octree();
    virtual ~Octree();
    void insert(CelestialBody *body);
    void erase(CelestialBody *body);
    void create_space();
    void calc_forces_per_body(CelestialBody *body);

    private:
    NodeOctree* root;
    NodeOctree* recursively_locate_node_father(NodeOctree* node, CelestialBody* body);
    NodeOctree* select_child(NodeOctree* node, CelestialBody* body);
    NodeOctree* recursively_locate_body(NodeOctree *node, CelestialBody *body);
    void recursively_insert(NodeOctree*&, CelestialBody *);
    bool recursively_erase(NodeOctree*&, CelestialBody *);
    void recursively_calc_forces(const NodeOctree *node, CelestialBody *body);
    uint8_t octant_for_position(const Vec3 &pos, const Vec3& center);
};


#endif //SIMULADORGRAVITACIONAL_OCTREE_H