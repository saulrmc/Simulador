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
    const NodeOctree* n_child(int index) const;

    friend class Octree;

    //exclusivo de este problema (osea del simulador)
    double get_body_mass() const;
    double get_body_radius() const;
    Vec3 get_node_center() const;
    double get_node_size() const;

private:
    void create_children();
    ElementOctree element_octree;
    NodeOctree *children[8]{};
};


#endif //SIMULADORGRAVITACIONAL_NODEOCTREE_H