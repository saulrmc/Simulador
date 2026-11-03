//
// Created by saul on 23/07/26.
//
#include "Octree.h"

void Octree::recursive_refresh_nodes(NodeOctree* node) {
    if (!node) return;
    if (node->has_children()) {
        for (auto& i : node->children) {
            recursive_refresh_nodes(i);
        }
        node->calc_avg_values();
        node->calc_max_radius();
    }
}

void Octree::iterative_insert(const CelestialBodies& bodies, int indexBody) {
    NodeOctree* node = root;
    int depth = 0;
    while (true) {
        if (!node->has_children()) {
            if ((int)node->element_octree.indexBodies.size() < CAPACITY || depth >= MAX_DEPTH) {
                double new_mass = node->element_octree.mass + bodies.get_mass(indexBody);
                node->element_octree.centerOfMass =
                    (node->element_octree.centerOfMass * node->element_octree.mass
                    + bodies.get_position(indexBody) * bodies.get_mass(indexBody)) / new_mass;
                node->element_octree.mass = new_mass;
                if (bodies.get_radius(indexBody) > node->element_octree.maxRadius) {
                    node->element_octree.maxRadius = bodies.get_radius(indexBody);
                }
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
                node->element_octree.maxRadius = -1;
                for (int oldIdx : oldIndices) {
                    int octant = octant_for_position(
                        bodies.get_position(oldIdx),
                        node->element_octree.center);
                    node->children[octant]->element_octree.indexBodies.push_back(oldIdx);
                    node->children[octant]->element_octree.mass += bodies.get_mass(oldIdx);
                    if (bodies.get_radius(oldIdx) > node->children[octant]->element_octree.maxRadius) {
                        node->children[octant]->element_octree.maxRadius = bodies.get_radius(oldIdx);
                    }
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
        ++depth;
    }
}

// TODO: habría que probar si es más eficiente pasar newForce como Vec3 o como doubles independientes

inline void Octree::recursively_calc_forces(const NodeOctree* node,
    CelestialBodies& bodies, int bodyIndex, Vec3 &newForce) {
    if (!node || node->element_octree.mass == 0) return;
    const double x = bodies.get_posX(bodyIndex);
    const double y = bodies.get_posY(bodyIndex);
    const double z = bodies.get_posZ(bodyIndex);
    const double m = bodies.get_mass(bodyIndex);


    const double d_x = x - node->element_octree.centerOfMass.get_x();
    const double d_y = y - node->element_octree.centerOfMass.get_y();
    const double d_z = z - node->element_octree.centerOfMass.get_z();
    const double d_2 = d_x * d_x + d_y * d_y + d_z * d_z;
    const double size_2 = node->element_octree.size * node->element_octree.size;

    // podría darse el caso de que el propio nodo (no necesariamente hoja) que contiene al cuerpo
    // cumpla esta condición así que se debe verificar que ese nodo NO
    // contenga al cuerpo en cuestión. Caso contrario se debe seguir
    // bajando por la estructura
    const double half = node->element_octree.size * 0.5;
    const Vec3& c = node->element_octree.center;
    if (size_2 < d_2 * theta2) {
        const bool contains =
        std::abs(x - c.get_x()) <= half &&
        std::abs(y - c.get_y()) <= half &&
        std::abs(z - c.get_z()) <= half;
        if (!contains) {
            newForce = newForce + force_exerted_from_to(
            node->element_octree.mass,
            node->element_octree.centerOfMass,
            m,
            Vec3(x, y, z)
            );
            return;
        }
        //si lo contiene entonces no se debe aproximar nada
    }
    if (!node->has_children()) {
        for (int i = 0; i < node->element_octree.indexBodies.size(); i++) {
            int otherIndex = node->element_octree.indexBodies[i];
            if (otherIndex != bodyIndex) {
                newForce = newForce + force_exerted_from_to(
                    bodies.get_mass(otherIndex),
                    bodies.get_position(otherIndex),
                    bodies.get_mass(bodyIndex),
                    bodies.get_position(bodyIndex)
                    );
            }
        }
        return;
    }
    for (int i = 0; i < 8; i++)
        recursively_calc_forces(node->children[i], bodies, bodyIndex, newForce);
}

void Octree::insert_impl(CelestialBodies& bodies) {
    for (int i = 0; i < bodies.size(); i++) {
        iterative_insert(bodies, i);
    }
}

void Octree::calc_forces_impl(CelestialBodies& bodies) {
    for (int i = 0; i < bodies.size(); i++) {
        Vec3 newForce(0, 0, 0);
        recursively_calc_forces(root, bodies, i, newForce);
        bodies.set_force(newForce, i);
    }
}