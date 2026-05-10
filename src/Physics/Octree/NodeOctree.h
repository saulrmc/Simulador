//
// Created by Saúl on 8/01/2026.
//

#ifndef SIMULADORGRAVITACIONAL_NODEOCTREE_H
#define SIMULADORGRAVITACIONAL_NODEOCTREE_H
#include "ElementOctree.h"
template<typename T>
class Octree;

template<typename T>
class NodeOctree {
public:
    NodeOctree();
    virtual ~NodeOctree();
    bool has_children() const;
    void calc_avg_values();
    const NodeOctree<T>* n_child(int index) const;

    friend class Octree<T>;

    //exclusivo de este problema (osea del simulador)
    double get_node_centerX() const;
    double get_node_centerY() const;
    double get_node_centerZ() const;
    double get_node_size() const;

private:
    void create_children();

    ElementOctree<T> element_octree;
    NodeOctree<T> *children[8]{};
    //std::vector<NodeOctree*> children;
};
#include "NodeOctree.tpp"
#endif //SIMULADORGRAVITACIONAL_NODEOCTREE_H
