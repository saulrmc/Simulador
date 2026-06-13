//
// Created by Saúl on 8/01/2026.
//
#pragma once

#include "Octree.h"

//static constexpr double THETA = 0;
// DEBUG
// inline long long visited_nodes = 0;
// inline long long leaf_nodes = 0;
// inline long long condition_calls = 0;
// inline long long condition_passed = 0;
// inline long long body_checks = 0;
// inline long long total_nodes = 0;


double Octree::get_size() const {
    return size;
}


void Octree::set_size(const double size) {
    this->size = size;
}


double Octree::get_centerX() const {
    return centerX;
}


double Octree::get_centerY() const {
    return centerY;
}


double Octree::get_centerZ() const {
    return centerZ;
}


void Octree::set_center(double centerX, double centerY, double centerZ) {
    this->centerX = centerX;
    this->centerY = centerY;
    this->centerZ = centerZ;
}


int Octree::get_num_bodies() const {
    return num_bodies;
}


double Octree::get_theta() const {
    return theta;
}
//
// 
// void Octree::calc_frontier_values() {
//     root->calc_frontier_values();
// }


Octree::Octree() {
    root = nullptr;
    num_bodies = 0;
    theta = 0;
    size = 0;
}


Octree::~Octree() {
    delete root;
}


void Octree::insertImpl(Vec3 &position, double mass) {
    iterative_insert(position, mass);
    //recursively_insert_2(body);
}


void Octree::erase(T *celestial_body) {
    recursively_erase(root, celestial_body);
}


void Octree::create_space() {
    if (root != nullptr) delete root;
    root = new NodeOctree();
    root->element_octree.size = this->size;
    root->element_octree.centerX = this->centerX;
    root->element_octree.centerY = this->centerY;
    root->element_octree.centerZ = this->centerZ;
    //root->calc_frontier_values();
}


void Octree::calc_forces_per_body(T *body) {
    body->set_force(Vec3(0, 0, 0)); //cada fuerza calculada no debe calcularse con la de un tiempo t anterior
    //es decir, no debe ser una acumulación temporal de la fuerza por cuerpo.
    recursively_calc_forces(root, body);
}


NodeOctree * Octree::locate_node_father(T *body) {
    NodeOctree* father = nullptr; //para no entregar el puntero original
    father = recursively_locate_node_father(root, body);
    return father;
}


NodeOctree * Octree::locate_body(T *body) {
    NodeOctree* node = nullptr;
    node = recursively_locate_body(root, body);
    return node;
}


void Octree::query_region(bool (*condition)(NodeOctree *const &,  T *const&),
    void (*action)(T *&, T *&, std::vector<T*>& ),
    T *body, std::vector<T*>& bodies) {
    recursive_query_region(root, condition, action, body, bodies);
}


void Octree::refresh_theta_value() {
    if (num_bodies < 100) this->theta = 0;
    else if (num_bodies < 1000) this-> theta = 0.3;//habria que testar cuantos cuerpos calculados por fuerza bruta
    //puede soportar la computadora de manera fluida
    else if (num_bodies < 10000) this-> theta = 0.5;
    else if (num_bodies < 100000) this-> theta = 0.6;
    else if (num_bodies < 1000000) this-> theta = 0.7;
    else theta = 0.8;
}


void Octree::refresh_mass_centers() {
    recursive_refresh_mass_centers(root);
}


void Octree::recursive_refresh_mass_centers(NodeOctree *node) {
    if (!node) return;
    if (node->has_children()) {
        for (auto & i : node->children) {
            recursive_refresh_mass_centers(i);
        }
        node->calc_avg_values();
    }
}


void Octree::recursive_query_region(NodeOctree *node,
    bool (*condition)(NodeOctree *const &, T*const&),
    void (*action)(T *&, T *&, std::vector<T*>& ),
    T *body, std::vector<T*>& bodies) {
    //visited_nodes++;
    if (!node or !condition or !action or !body) return;
    //condition_calls++;
    if (condition(node, body) == false) return;
    //condition_passed++;
    if (node->has_children()) {
        // int index = octant_for_position(
        //     body->get_posX(),
        //     body->get_posY(),
        //     body->get_posZ(),
        //     node->element_octree.get_center_x(),
        //     node->element_octree.get_center_y(),
        //     node->element_octree.get_center_z());
        for (int index = 0; index < 8; index++)
            recursive_query_region(node->children[index], condition, action, body, bodies);
    }
    else {
        //leaf_nodes++;
        //body_checks += node->element_octree.bodies.size();
        for (int i = 0; i < node->element_octree.bodies.size(); i++) {
            if (node->element_octree.bodies[i] and node->element_octree.bodies[i] != body and
            body->get_index() < node->element_octree.bodies[i]->get_index())
                action(node->element_octree.bodies[i], body, bodies);
        }
    }
}


NodeOctree * Octree::recursively_locate_body(NodeOctree *node, T *body) {
    if (!node) return nullptr;
    Vec3 pos = body->get_position();
    Vec3 center = node->element_octree.get_position();

    //codigo repetido de la función select_child xd
    if (node->has_children()){
        uint8_t index = octant_for_position(pos, center);
        return recursively_locate_body(node->children[index], body);
    }
    for (int i = 0; i < CAPACITY; i++) {
        if (node->element_octree.bodies[i] == body) return node;
    }
    return nullptr;
}


NodeOctree * Octree::recursively_locate_node_father(NodeOctree *node, T *body) {
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


NodeOctree * Octree::select_child(NodeOctree *node, T *body) {
    Vec3 pos = body->get_position();
    Vec3 center = node->element_octree.get_position();

    uint8_t index = octant_for_position(pos, center);

    return node->children[index];
}


void Octree::iterative_insert(Vec3& position, double bodyMass) {
    NodeOctree *node_octree = root;
    std::vector<NodeOctree *> stack{};
    stack.push_back(node_octree);
    while (!stack.empty()) {
        if (!node_octree->has_children()) { //si es un nodo hoja
            if (node_octree->element_octree.bodies.size() == CAPACITY) {//si está lleno
                node_octree->create_children();
            }
            else { //agregar
                double new_mass = node_octree->element_octree.mass + bodyMass;
                node_octree->
            }
        }
    }







    if (!node_octree->has_children()) {//si el nodo es externo...
        if (node_octree->element_octree.bodies.size() == CAPACITY) {//si está lleno
            node_octree->create_children();
            //total_nodes+=CAPACITY;
            for (int i = 0; i < CAPACITY; i++) {
                T* oldBody = node_octree->element_octree.bodies[i];
                uint8_t index = octant_for_position(
                    oldBody->get_posX(),
                    oldBody->get_posY(),
                    oldBody->get_posZ(),
                    node_octree->element_octree.get_center_x(),
                    node_octree->element_octree.get_center_y(),
                    node_octree->element_octree.get_center_z());
                num_bodies--;
                //NodeOctree *destinyOld = select_child(node_octree, oldBody);
                recursively_insert(node_octree->children[index], oldBody);
            }
            node_octree->element_octree.bodies.clear();
        }
        else {
            //body->set_id(num_id);
            //como el nodo externo recien tiene un cuerpo entonces
            //se debe inicializar los valores del centro de masa y masa
            //total del nodo para que no estén en 0 cuando se le quiera
            //ingresar otro cuerpo en el mismo nodo
            double new_mass = node_octree->element_octree.mass + bodies->get_mass();

            node_octree->element_octree.centerOfMassX =
                (node_octree->element_octree.centerOfMassX * node_octree->element_octree.mass
                    + bodies->get_posX() * bodies->get_mass())/ new_mass;
            node_octree->element_octree.centerOfMassY =
                (node_octree->element_octree.centerOfMassY * node_octree->element_octree.mass
                    + bodies->get_posY() * bodies->get_mass())/ new_mass;
            node_octree->element_octree.centerOfMassZ =
                (node_octree->element_octree.centerOfMassZ * node_octree->element_octree.mass
                    + bodies->get_posZ() * bodies->get_mass())/ new_mass;

            node_octree->element_octree.mass = new_mass;
            node_octree->element_octree.bodies.push_back(bodies);

            ++this->num_bodies;
            //this->num_id++;
            return;
        }
    }
    //NodeOctree *destiny_new = select_child(node_octree, body);
    uint8_t index = octant_for_position(
        bodies->get_posX(),
        bodies->get_posY(),
        bodies->get_posZ(),
        node_octree->element_octree.get_center_x(),
        node_octree->element_octree.get_center_y(),
        node_octree->element_octree.get_center_z());

    recursively_insert(node_octree->children[index], bodies);
    node_octree->calc_avg_values();
    node_octree->element_octree.bodies.clear();
}


void Octree::recursively_insert_2(T *&body) {
    //Esta es una función mayormente iterativa que, por simplicidad, se
    //llama recursivamente algunas veces. No es tan eficiente como
    //una recursión pura, pero se evita repetir el bucle de inserción
    //dentro de sí mismo.
    std::vector<NodeOctree*> ancestors;
    NodeOctree *node = root;
    while (true) {
        if (!node->has_children()) {
            if (node->element_octree.bodies.size() == CAPACITY) {
                node->create_children();
                std::vector<T*> oldBodies = node->element_octree.bodies;
                num_bodies-=static_cast<int>(node->element_octree.bodies.size());
                node->element_octree.bodies.clear();
                for (T *& oldBody : oldBodies) {
                    recursively_insert_2(oldBody);
                }
            }
            else {
                double new_mass = node->element_octree.mass + body->get_mass();
                node->element_octree.centerOfMass = (node->element_octree.centerOfMass
                    * node->element_octree.mass + body->get_position() * body->get_mass()) / new_mass;
                node->element_octree.mass = new_mass;
                node->element_octree.bodies.push_back(body);
                ++this->num_bodies;
                break;
            }
        }
        const int octant = octant_for_position(body->get_position(),
            node->element_octree.get_position());
        ancestors.push_back(node); //"guarda" un rastro de los ancestros del nodo
        node = node->children[octant];
    }
    while (!ancestors.empty()) {
        NodeOctree *ancestor = ancestors.back();
        ancestor->calc_avg_values();
        ancestors.pop_back();
    }
    ancestors.clear();
}


bool Octree::recursively_erase(NodeOctree *&node_octree, T *body) {
    if (!node_octree) return false;
    //nodo externo
    if (!node_octree->has_children()) {

        if (!node_octree->element_octree.bodies.empty()) {
            for (int i = 0; i < CAPACITY; i++) {
                if (node_octree->element_octree.bodies[i] == body) {
                    T *erase = node_octree->element_octree.bodies[i];
                    node_octree->element_octree.bodies[i] = node_octree->element_octree.bodies.back();
                    node_octree->element_octree.bodies.pop_back();
                    delete erase;
                    node_octree->element_octree.mass = 0;
                    node_octree->element_octree.centerOfMass = Vec3(0,0,0);
                    --this->num_bodies;
                    return true;
                }
            }
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


void Octree::recursively_calc_forces(const NodeOctree *node_octree, T *body) { //esta funcion va a servir
    //para acumular todas las fuerzas de todos los nodos sobre este cuerpo

    //distancia del cuerpo al centro de masa del nodo
    if (!node_octree || node_octree->element_octree.mass == 0) return;

    //const double d = (body->get_position() - node_octree->element_octree.centerOfMass).magnitude();

    //una raiz cuadrada es menos eficiente que elevar al cuadrado
    // const double d_2 = pow(body->get_posX() - node_octree->element_octree.centerOfMassX, 2)
    //     + pow(body->get_posY() - node_octree->element_octree.centerOfMassY, 2)
    //     + pow(body->get_posZ() - node_octree->element_octree.centerOfMassZ, 2);
    const double d_x = body->get_posX() - node_octree->element_octree.centerOfMassX;
    const double d_y = body->get_posY() - node_octree->element_octree.centerOfMassY;
    const double d_z = body->get_posZ() - node_octree->element_octree.centerOfMassZ;
    const double d_2 = d_x * d_x + d_y * d_y + d_z * d_z;

    if (d_2 == 0) return; //para evitar una division entre 0

    if (node_octree->element_octree.size * node_octree->element_octree.size < d_2 * theta * theta) {
        //significa que el nodo está lo suficientemente lejos
        //y se puede tratar como un solo cuerpo
        Vec3 new_force = force_exerted_from_to(
            node_octree->element_octree.mass,
            Vec3(node_octree->element_octree.centerOfMassX,
                node_octree->element_octree.centerOfMassY,
                node_octree->element_octree.centerOfMassZ),
            body->get_mass(),
            body->get_position()
            );
        body->set_force(body->get_force() + new_force);
    }
    else if (!node_octree->has_children()) {
        for (int i = 0; i < node_octree->element_octree.bodies.size(); i++) {
            if (node_octree->element_octree.bodies[i] != body) {
                Vec3 new_force = force_exerted_from_to(
                node_octree->element_octree.bodies[i]->get_mass(),
                node_octree->element_octree.bodies[i]->get_position(), //1
                body->get_mass(),body->get_position() //2
                );
                body->set_force(body->get_force() + new_force);
            }
        }
    }
    else for (int i=0; i < 8;i++) recursively_calc_forces(node_octree->children[i], body);
}


void Octree::iterative_calc_forces(T *body) {
    NodeOctree node = root;

}


uint8_t Octree::octant_for_position(double posX, double posY, double posZ,
        double centerX, double centerY, double centerZ){
    uint8_t index = 0;
    //se le va agregando bits 1's mientras va cumpliendo las condiciones.
    //Depende directamente de cómo están ordenados los nodos hijos en la función
    //NodeOctree::create_children()
    if (posX >= centerX) index |= 1;
    if (posY >= centerY) index |= 2;
    if (posZ >= centerZ) index |= 4;
    return index;
}