//
// Created by Saúl on 8/01/2026.
//

#include "Octree.h"
//static constexpr double THETA = 0;

int Octree::get_num_bodies() const {
    return num_bodies;
}

double Octree::get_theta() const {
    return theta;
}

Octree::Octree() {
    root = nullptr;
    num_bodies = 0;
    theta = 0;
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
    root->element_octree.size = 80000; //en unidades de distancia xd
    root->element_octree.center = Vec3(0, 0, 0);
}

void Octree::calc_forces_per_body(CelestialBody *body) {
    body->set_force(Vec3(0, 0, 0)); //cada fuerza calculada no debe calcularse con la de un tiempo t anterior
    //es decir, no debe ser una acumulación temporal de la fuerza por cuerpo.
    recursively_calc_forces(root, body);
}

NodeOctree * Octree::locate_node_father(CelestialBody *body) {
    NodeOctree* father = nullptr; //para no entregar el puntero original
    father = recursively_locate_node_father(root, body);
    return father;
}

NodeOctree * Octree::locate_body(CelestialBody *body) {
    NodeOctree* node = nullptr;
    node = recursively_locate_body(root, body);
    return node;
}

void Octree::query_region(NodeOctree *&node, void *(*condition)(const Vec3 &, double, double), void *(*action)()) {
    if (!node) return;
    if (!condition) return;
    if (node->has_children()) {
        for (int i = 0; i < 8; i++) query_region(node->children[0], condition, action);
    }
    action();
}


NodeOctree * Octree::recursively_locate_body(NodeOctree *node, CelestialBody *body) {
    if (!node) return nullptr;
    Vec3 pos = body->get_position();
    Vec3 center = node->element_octree.get_position();

    //codigo repetido de la función select_child xd
    if (node->has_children()){
        uint8_t index = octant_for_position(pos, center);
        return recursively_locate_body(node->children[index], body);
    }
    if (node->element_octree.body == body) return node;
    return nullptr;
}


NodeOctree * Octree::recursively_locate_node_father(NodeOctree *node, CelestialBody *body) {
    if (!node || !node->has_children()) return nullptr;
    Vec3 pos = body->get_position();
    Vec3 center = node->element_octree.get_position();
    //codigo repetido de la función select_child xd
    //el chequeo es basándose en la posición del cuerpo y el centro del nodo
    //pero nunca se verifica explícitamente que los punteros *body y el
    //puntero al cuerpo que tenga uno de los hijos del nodo apunten al mismo cuerpo
    //por lo que es más una confianza ciega en que la función octant_for_position hace
    //bien su trabajo y que además el octree funciona correctamente. Esto en realidad es válido
    //porque no es responsabilidad de esta función asegurarse que el resto funcione bien
    //y además ahora tener que ejecutar bucles para buscar entre los 8 hijos del nodo seleccionado
    uint8_t index = octant_for_position(pos, center);

    if (!node->children[index]) return nullptr;
    if (!node->children[index]->has_children()) return node;
    return recursively_locate_body(node->children[index], body);
}

NodeOctree * Octree::select_child(NodeOctree *node, CelestialBody *body) {
    Vec3 pos = body->get_position();
    Vec3 center = node->element_octree.get_position();

    uint8_t index = octant_for_position(pos, center);

    return node->children[index];
}

void Octree::recursively_insert(NodeOctree *&node_octree, CelestialBody *body) {
    if (!node_octree->has_children()) {//si el nodo es externo...
        if (node_octree->element_octree.body) {
            CelestialBody* oldBody = node_octree->element_octree.body;
            node_octree->element_octree.body = nullptr;
            node_octree->create_children();
            NodeOctree *destinyOld = select_child(node_octree, oldBody);
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

            this->num_bodies++;
            if (num_bodies < 100) this->theta = 0;
            else if (num_bodies >= 100 and num_bodies < 1000) this-> theta = 0.1;//habria que testar cuantos cuerpos calculados por fuerza bruta
            //puede soportar la computadora de manera fluida
            else if (num_bodies >= 1000 and num_bodies < 10000) this-> theta = 0.2;
            else if (num_bodies >= 10000 and num_bodies < 100000) this-> theta = 0.3;
            else if (num_bodies >= 100000 and num_bodies < 1000000) this-> theta = 0.5;
            else theta = 0.7;
            return;
        }
    }
    NodeOctree *destiny_new = select_child(node_octree, body);
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
            this->num_bodies--;
            return true;
        }
        return false;
    }
    NodeOctree *destiny = select_child(node_octree, body);
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
    else if (node_octree->element_octree.size/d < theta) { //significa que el nodo está lo suficientemente lejos
        //y se puede tratar como un solo cuerpo
        Vec3 new_force = force_exerted_from_to(
            node_octree->element_octree.mass, node_octree->element_octree.centerOfMass,
            body->get_mass(), body->get_position()
            );
        body->set_force(bodyCurrentForce + new_force);
    }
    else for (int i=0; i < 8;i++) recursively_calc_forces(node_octree->children[i], body);
}

uint8_t Octree::octant_for_position(const Vec3& pos, const Vec3& center) {
    uint8_t index = 0;
    //se le va agregando bits 1's mientras va cumpliendo las condiciones.
    //Depende directamente de cómo están ordenados los nodos hijos en la función
    //NodeOctree::create_children()
    if (pos.get_x() >= center.get_x()) index |= 1;
    if (pos.get_y() >= center.get_y()) index |= 2;
    if (pos.get_z() >= center.get_z()) index |= 4;
    return index;
}
