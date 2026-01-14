//
// Created by Saúl on 8/01/2026.
//

#ifndef SIMULADORGRAVITACIONAL_OCTREE_H
#define SIMULADORGRAVITACIONAL_OCTREE_H
#include "NodeOctree.h"
#include "../Physics.cpp"

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

    NodeOctree* locate_body(NodeOctree* node, CelestialBody* body);
    void recursively_insert(NodeOctree*&, CelestialBody *);
    bool recursively_erase(NodeOctree*&, CelestialBody *);
    void recursively_calc_forces(NodeOctree *node, CelestialBody *body);
};


#endif //SIMULADORGRAVITACIONAL_OCTREE_H