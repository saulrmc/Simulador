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

void Octree::insert(CelestialBody *body) {
    recursively_insert(root, body);
}

void Octree::erase() {
}

void Octree::create_space() {

}


NodeOctree * Octree::locate_body(NodeOctree *node, CelestialBody *body) {
    Vec3 pos = body->get_position();
    Vec3 center = node->element_octree.get_position();

    //se le va agregando bits 1's mientras va cumpliendo las condiciones.
    //Depende directamente de cómo están ordenados los nodos hijos en la función
    //NodeOctree::create_children()
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
            NodeOctree *destiny_old = locate_body(node_octree, oldBody);
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
    NodeOctree *destiny_new = locate_body(node_octree, body);
    recursively_insert(destiny_new, body);
    node_octree->calc_avg_values();
    node_octree->element_octree.body = nullptr;
}

bool Octree::recursively_erase(NodeOctree *&node_octree, CelestialBody *body) {
    if (!node_octree) return false;
    //nodo externo
    if (!node_octree->has_children()) {
        if (node_octree->element_octree.body == body) { //falta implementar esta sobrecarga
            node_octree->element_octree.body = nullptr;
            node_octree->element_octree.mass = 0;
            node_octree->element_octree.centerOfMass = Vec3(0,0,0);
            return true;
        }
        return false;
    }
    NodeOctree *destiny = locate_body(node_octree, body);
    if (!destiny) return false; //para evitar los nullptr
    bool erased = recursively_erase(destiny, body);
    if (erased) node_octree->calc_avg_values(); //lógico... solo se debería recalcular los
    //datos del nodo padre si es que se ha borrado un cuerpo de sus hijos
    return erased;
}

