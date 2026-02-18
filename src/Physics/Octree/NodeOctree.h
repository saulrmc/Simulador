//
// Created by Saúl on 8/01/2026.
//

#ifndef SIMULADORGRAVITACIONAL_NODEOCTREE_H
#define SIMULADORGRAVITACIONAL_NODEOCTREE_H
#include "ElementOctree.h"
class NodeOctree {
public:
    NodeOctree();
    virtual ~NodeOctree();
    bool has_children() const;
    void calc_avg_values();

    friend class Octree;

private:
    void create_children();
    ElementOctree element_octree;
    NodeOctree *children[8]{};
};


#endif //SIMULADORGRAVITACIONAL_NODEOCTREE_H