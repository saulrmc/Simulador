//
// Created by saul on 23/07/26.
//
#include "Octree.h"

void Octree::recursive_refresh_mass_centers(NodeOctree* node) {
    if (!node) return;
    if (node->has_children()) {
        for (auto& i : node->children) {
            recursive_refresh_mass_centers(i);
        }
        node->calc_avg_values();
    }
}

void Octree::iterative_insert(const CelestialBodies& bodies, int indexBody) {
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

void Octree::insert_impl(CelestialBodies& bodies) {
    for (int i = 0; i < bodies.size(); i++) {
        iterative_insert(bodies, i);
    }
}

void Octree::calc_forces_impl(CelestialBodies& bodies) {
    for (int i = 0; i < bodies.size(); i++) {
        bodies.set_force(Vec3(0, 0, 0), i);
        recursively_calc_forces(root, bodies, i);
    }
}