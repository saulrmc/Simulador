#pragma once
#ifndef SIMULADORGRAVITACIONAL_OCTREE_H
#define SIMULADORGRAVITACIONAL_OCTREE_H
#include <cstdint>
#include <vector>

#include "NodeOctree.h"
#include "../SpatialStructure.h"
#include "../geometry.h"
#include "../../Physics.h"
#include "CelestialBodies.h"

class Octree : public SpatialStructure<Octree> {
    friend class SpatialStructure<Octree>;
public:
    Octree();
    ~Octree();

private:
    void create_space_impl();
    void insert_impl(CelestialBodies& bodies);
    void calc_forces_impl(CelestialBodies& bodies);
    void refresh_mass_centers_impl();
    void refresh_theta_value_impl();
    void set_size_impl(double s);
    void set_center_impl(Vec3 c);
    double get_size_impl() const;
    Vec3 get_center_impl() const;
    double get_theta_impl() const;

    template<typename Callback>
    void query_region_impl(Callback&& on_pair, CelestialBodies& bodies, int bodyIndex) {
        recursive_query_region(root, std::forward<Callback>(on_pair), bodies, bodyIndex);
    }

    NodeOctree* root;
    void iterative_insert(const CelestialBodies& bodies, int indexBody);
    void recursively_calc_forces(const NodeOctree* node, CelestialBodies& bodies, int bodyIndex);
    uint8_t octant_for_position(Vec3 position, Vec3& center);

    template<typename Callback>
    void recursive_query_region(NodeOctree* node, Callback&& on_pair,
        CelestialBodies& bodies, int bodyIndex) {
        if (!node) return;
        if (!overlap_node(node->element_octree.center, node->element_octree.size,
                bodies, bodyIndex))
            return;
        if (node->has_children()) {
            for (int i = 0; i < 8; i++)
                recursive_query_region(node->children[i], on_pair, bodies, bodyIndex);
        }
        else {
            for (int i = 0; i < node->element_octree.indexBodies.size(); i++) {
                int otherIndex = node->element_octree.indexBodies[i];
                if (otherIndex != bodyIndex && bodyIndex < otherIndex)
                    on_pair(bodies, bodyIndex, otherIndex);
            }
        }
    }

    void recursive_refresh_mass_centers(NodeOctree* node);
    int num_bodies;
    double theta = 0;
    double size;
    Vec3 center;
};


inline Octree::Octree() {
    root = nullptr;
    num_bodies = 0;
    theta = 0;
    size = 0;
}

inline Octree::~Octree() {
    delete root;
}

inline void Octree::create_space_impl() {
    if (root != nullptr) delete root;
    root = new NodeOctree();
    root->element_octree.size = this->size;
    root->element_octree.center = this->center;
}


inline void Octree::refresh_mass_centers_impl() {
    recursive_refresh_mass_centers(root);
}

inline void Octree::refresh_theta_value_impl() {
    if (num_bodies < 100) this->theta = 0;
    else if (num_bodies < 1000) this->theta = 0.3;
    else if (num_bodies < 10000) this->theta = 0.5;
    else if (num_bodies < 100000) this->theta = 0.6;
    else if (num_bodies < 1000000) this->theta = 0.7;
    else theta = 0.8;
}

inline void Octree::set_size_impl(double s) {
    this->size = s;
}

inline void Octree::set_center_impl(Vec3 c) {
    this->center = c;
}

inline double Octree::get_size_impl() const {
    return this->size;
}

inline Vec3 Octree::get_center_impl() const {
    return this->center;
}

inline double Octree::get_theta_impl() const {
    return this->theta;
}

inline uint8_t Octree::octant_for_position(Vec3 position, Vec3& center) {
    uint8_t index = 0;
    if (position.get_x() >= center.get_x()) index |= 1;
    if (position.get_y() >= center.get_y()) index |= 2;
    if (position.get_z() >= center.get_z()) index |= 4;
    return index;
}

#endif //SIMULADORGRAVITACIONAL_OCTREE_H
