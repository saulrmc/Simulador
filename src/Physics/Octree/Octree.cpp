//
// Created by Saúl on 8/01/2026.
//

#include "Octree.h"

#include <cstdint>

Octree::Octree() {
    root = nullptr;
}

Octree::~Octree() {
}

void Octree::insert() {
}

void Octree::erase() {
}

void Octree::create_space() {

}


NodeOctree * Octree::locate_body(NodeOctree *node, CelestialBody *body) {
    Vec3 pos = body->get_position();
    Vec3 center = node->element_octree.get_position();

    uint8_t index = 0;
    if (pos.get_x() >= center.get_x()) index |= 1;
    if (pos.get_y() >= center.get_y()) index |= 2;
    if (pos.get_z() >= center.get_z()) index |= 4;

    return node->children[index];
}

void Octree::recursively_insert(NodeOctree *&node_octree, CelestialBody *body) {
    if (!node_octree->has_children()) {//si el nodo es externo...
        if (node_octree->element_octree.body) {
            CelestialBody* oldBody = node_octree->element_octree.body;
            node_octree->element_octree.body = nullptr;
            node_octree->create_children();
            NodeOctree *destiny_old = locate_body(oldBody);
            recursively_insert(destiny_old, oldBody);
        }
        else {
            node_octree->element_octree.body = body;
            //como el nodo externo recien tiene un cuerpo entonces
            //se debe incializar los valores del centro de masa y masa
            //total del nodo para que no estén en 0 cuando se le quiera
            //ingresar otro cuerpo en el mismo nodo
            node_octree->element_octree.mass = body->get_mass();
            node_octree->element_octree.centerOfMass = body->get_position();
            return;
        }
    }
    NodeOctree *destiny_new = locate_body(body);
    recursively_insert(destiny_new, body);
    node_octree->calc_avg_values();
    node_octree->element_octree.body = nullptr;
}

void Octree::recursively_erase(NodeOctree *&node_octree, CelestialBody *body) {
}

