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
    void insert_impl(const CelestialBodies& bodies);
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

inline void Octree::insert_impl(const CelestialBodies& bodies) {
    for (int i = 0; i < bodies.size(); i++) {
        iterative_insert(bodies, i);
    }
}

inline void Octree::calc_forces_impl(CelestialBodies& bodies) {
    for (int i = 0; i < bodies.size(); i++) {
        bodies.set_force(Vec3(0, 0, 0), i);
        recursively_calc_forces(root, bodies, i);
    }
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

inline void Octree::recursive_refresh_mass_centers(NodeOctree* node) {
    if (!node) return;
    if (node->has_children()) {
        for (auto& i : node->children) {
            recursive_refresh_mass_centers(i);
        }
        node->calc_avg_values();
    }
}

inline void Octree::iterative_insert(const CelestialBodies& bodies, int indexBody) {
    NodeOctree* node = root;

    while (true) {
        if (!node->has_children()) {
            if ((int)node->element_octree.indexBodies.size() < CAPACITY) {
                double new_mass = node->element_octree.mass + bodies.get_mass(indexBody);
                node->element_octree.centerOfMass =
                    (node->element_octree.centerOfMass * node->element_octree.mass
                    + bodies.get_position(indexBody) * bodies.get_mass(indexBody)) / new_mass;
                node->element_octree.mass = new_mass;
                node->element_octree.indexBodies.push_back(indexBody);
                ++this->num_bodies;
                return;
            }
            else {
                node->create_children();
                std::vector<int> oldIndices = node->element_octree.indexBodies;
                num_bodies -= static_cast<int>(oldIndices.size());
                node->element_octree.indexBodies.clear();
                node->element_octree.mass = 0;
                node->element_octree.centerOfMass = Vec3(0, 0, 0);
                for (int oldIdx : oldIndices) {
                    int octant = octant_for_position(
                        bodies.get_position(oldIdx),
                        node->element_octree.center);
                    node->children[octant]->element_octree.indexBodies.push_back(oldIdx);
                    node->children[octant]->element_octree.mass += bodies.get_mass(oldIdx);
                    num_bodies++;
                }
                for (int i = 0; i < 8; i++) {
                    if (!node->children[i]->element_octree.indexBodies.empty()) {
                        node->children[i]->element_octree.centerOfMass = Vec3(0, 0, 0);
                        for (int idx : node->children[i]->element_octree.indexBodies) {
                            node->children[i]->element_octree.centerOfMass =
                                node->children[i]->element_octree.centerOfMass
                                + bodies.get_position(idx) * bodies.get_mass(idx);
                        }
                        node->children[i]->element_octree.centerOfMass =
                            node->children[i]->element_octree.centerOfMass
                            / node->children[i]->element_octree.mass;
                    }
                }
            }
        }

        int octant = octant_for_position(
            bodies.get_position(indexBody),
            node->element_octree.center);
        node = node->children[octant];
    }
}

inline void Octree::recursively_calc_forces(const NodeOctree* node, CelestialBodies& bodies, int bodyIndex) {
    if (!node || node->element_octree.mass == 0) return;

    const double d_x = bodies.get_posX(bodyIndex) - node->element_octree.centerOfMass.get_x();
    const double d_y = bodies.get_posY(bodyIndex) - node->element_octree.centerOfMass.get_y();
    const double d_z = bodies.get_posZ(bodyIndex) - node->element_octree.centerOfMass.get_z();
    const double d_2 = d_x * d_x + d_y * d_y + d_z * d_z;

    if (d_2 == 0) return;

    if (node->element_octree.size * node->element_octree.size < d_2 * theta * theta) {
        Vec3 new_force = force_exerted_from_to(
            node->element_octree.mass,
            node->element_octree.centerOfMass,
            bodies.get_mass(bodyIndex),
            bodies.get_position(bodyIndex)
        );
        bodies.set_force(bodies.get_force(bodyIndex) + new_force, bodyIndex);
    }
    else if (!node->has_children()) {
        for (int i = 0; i < node->element_octree.indexBodies.size(); i++) {
            int otherIndex = node->element_octree.indexBodies[i];
            if (otherIndex != bodyIndex) {
                Vec3 new_force = force_exerted_from_to(
                    bodies.get_mass(otherIndex),
                    bodies.get_position(otherIndex),
                    bodies.get_mass(bodyIndex),
                    bodies.get_position(bodyIndex)
                );
                bodies.set_force(bodies.get_force(bodyIndex) + new_force, bodyIndex);
            }
        }
    }
    else {
        for (int i = 0; i < 8; i++)
            recursively_calc_forces(node->children[i], bodies, bodyIndex);
    }
}

inline uint8_t Octree::octant_for_position(Vec3 position, Vec3& center) {
    uint8_t index = 0;
    if (position.get_x() >= center.get_x()) index |= 1;
    if (position.get_y() >= center.get_y()) index |= 2;
    if (position.get_z() >= center.get_z()) index |= 4;
    return index;
}

#endif //SIMULADORGRAVITACIONAL_OCTREE_H
