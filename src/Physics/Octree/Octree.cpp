//
// Created by Saúl on 8/01/2026.
//

#include "Octree.h"
#include <cstdint>
static constexpr double THETA = 0.5;

Octree::Octree() {
    root = nullptr;
}

Octree::~Octree() {
    delete root;
}

void Octree::insert(CelestialBody *body) {
    if (!root) create_space();
    recursively_insert(root, body);
}

void Octree::erase(CelestialBody *celestial_body) {
    recursively_erase(root, celestial_body);
}

void Octree::create_space() {
    if (root != nullptr) delete root;
    root = new NodeOctree();
    root->element_octree.size = 100; //en unidades de distancia xd
    root->element_octree.center = Vec3(0, 0, 0);
}

void Octree::calc_forces_per_body(CelestialBody *body) {
    body->set_force(Vec3(0, 0, 0)); //cada fuerza calculada no debe calcularse con la de un tiempo t anterior
    //es decir, no debe ser una acumulación temporal de la fuerza por cuerpo.
    recursively_calc_forces(root, body);
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
            NodeOctree *destinyOld = locate_body(node_octree, oldBody);
            recursively_insert(destinyOld, oldBody);
        }
        else {
            node_octree->element_octree.body = body;
            //como el nodo externo recien tiene un cuerpo entonces
            //se debe inicializar los valores del centro de masa y masa
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

        if (node_octree->element_octree.body and
            node_octree->element_octree.body == body) {

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

void Octree::recursively_calc_forces(const NodeOctree *node_octree, CelestialBody *body) { //esta funcion va a servir
    //para acumular todas las fuerzas de todos los nodos sobre este cuerpo

    //distancia del cuerpo al centro de masa del nodo
    if (!node_octree || node_octree->element_octree.mass == 0) return;

    const double d = (body->get_position() - node_octree->element_octree.centerOfMass).magnitude();
    if (d == 0) return; //para evitar una division entre 0

    Vec3 bodyCurrentForce = body->get_force();

    if (!node_octree->has_children() and node_octree->element_octree.body != body) {
        Vec3 new_force = force_exerted_from_to(
            node_octree->element_octree.body->get_mass(),node_octree->element_octree.body->get_position(), //1
            body->get_mass(),body->get_position() //2
            );
        body->set_force(bodyCurrentForce + new_force);
    }
    else if (node_octree->element_octree.size/d < THETA) { //significa que el nodo está lo suficientemente lejos
        //y se puede tratar como un solo cuerpo
        Vec3 new_force = force_exerted_from_to(
            node_octree->element_octree.mass, node_octree->element_octree.centerOfMass,
            body->get_mass(), body->get_position()
            );
        body->set_force(bodyCurrentForce + new_force);
    }
    else for (int i=0; i < 8;i++) recursively_calc_forces(node_octree->children[i], body);
}
