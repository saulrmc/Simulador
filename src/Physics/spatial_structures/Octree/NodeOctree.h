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
    Vec3 get_node_center() const;
    double get_node_size() const;
    // double get_frontier_x_min() const;
    // double get_frontier_y_min() const;
    // double get_frontier_z_min() const;
    // double get_frontier_x_max() const;
    // double get_frontier_y_max() const;
    // double get_frontier_z_max() const;
    // void calc_frontier_values();

private:
    void create_children();

    ElementOctree element_octree;
    NodeOctree *children[8]{};
    //std::vector<NodeOctree*> children;

    //el index 0 y 1 para xMin y xMax
    //el index 2 y 3 para yMin & yMax
    //el index 4 y 5 para zMin y zMax
    // double frontierValue[6]{};
};


inline NodeOctree::NodeOctree() {
}

inline bool NodeOctree::has_children() const {
    return this->children[0] != nullptr;
    //return !children.empty();
}

inline const NodeOctree * NodeOctree::n_child(int index) const {
    if (!this->has_children())return nullptr;
    NodeOctree * n_child = children[index];
    return n_child;
}


inline Vec3 NodeOctree::get_node_center() const {
    return this->element_octree.center;
}


inline double NodeOctree::get_node_size() const {
    return this->element_octree.size;
}
//
// 
// double NodeOctree::get_frontier_x_min() const {
//     return frontierValue[0];
// }
//
// 
// double NodeOctree::get_frontier_y_min() const {
//     return frontierValue[2];
// }
//
// 
// double NodeOctree::get_frontier_z_min() const {
//     return frontierValue[4];
// }
//
// 
// double NodeOctree::get_frontier_x_max() const {
//     return frontierValue[1];
// }
//
// 
// double NodeOctree::get_frontier_y_max() const {
//     return frontierValue[3];
// }
//
// 
// double NodeOctree::get_frontier_z_max() const {
//     return frontierValue[5];
// }
//
// 
// void NodeOctree::calc_frontier_values() {
//     double half_size = this->element_octree.size/2;
//     //minX
//     frontierValue[0] = this->element_octree.centerX - half_size;
//     //minY
//     frontierValue[2] = this->element_octree.centerY - half_size;
//     //minZ
//     frontierValue[4] = this->element_octree.centerZ - half_size;
//     //maxX
//     frontierValue[1] = this->element_octree.centerX + half_size;
//     //maxY
//     frontierValue[3] = this->element_octree.centerY + half_size;
//     //maxZ
//     frontierValue[5] = this->element_octree.centerZ + half_size;
// }
#endif //SIMULADORGRAVITACIONAL_NODEOCTREE_H
