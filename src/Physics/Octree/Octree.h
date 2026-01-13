//
// Created by Saúl on 8/01/2026.
//

#ifndef SIMULADORGRAVITACIONAL_OCTREE_H
#define SIMULADORGRAVITACIONAL_OCTREE_H
#include "NodeOctree.h"

class Octree {
    public:
    Octree();
    virtual ~Octree();
    void insert(CelestialBody *body);
    void erase();
    void create_space();

    private:
    NodeOctree* root;

    NodeOctree* locate_body(NodeOctree* node, CelestialBody* body);

    void recursively_insert(NodeOctree*&, CelestialBody *);
    bool recursively_erase(NodeOctree*&, CelestialBody *);
};


#endif //SIMULADORGRAVITACIONAL_OCTREE_H