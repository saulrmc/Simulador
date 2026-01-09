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
    void insert();
    void erase();
    void create_space();
    private:
    NodeOctree* root;
    void recursively_create_space(NodeOctree*);
    void recursively_insert(NodeOctree*);
    void recursively_erase(NodeOctree*);
};


#endif //SIMULADORGRAVITACIONAL_OCTREE_H