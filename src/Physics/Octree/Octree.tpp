//
// Created by Saúl on 8/01/2026.
//
#pragma once

#include "Octree.h"

//static constexpr double THETA = 0;
template<typename T>
double Octree<T>::get_size() const {
    return size;
}

template<typename T>
void Octree<T>::set_size(const double size) {
    this->size = size;
}

template<typename T>
double Octree<T>::get_centerX() const {
    return centerX;
}

template<typename T>
double Octree<T>::get_centerY() const {
    return centerY;
}

template<typename T>
double Octree<T>::get_centerZ() const {
    return centerZ;
}

template<typename T>
void Octree<T>::set_center(double centerX, double centerY, double centerZ) {
    this->centerX = centerX;
    this->centerY = centerY;
    this->centerZ = centerZ;
}

template<typename T>
int Octree<T>::get_num_bodies() const {
    return num_bodies;
}

template<typename T>
double Octree<T>::get_theta() const {
    return theta;
}

template<typename T>
Octree<T>::Octree() {
    root = nullptr;
    num_bodies = 0;
    theta = 0;
    size = 0;
}

template<typename T>
Octree<T>::~Octree() {
    delete root;
}

template<typename T>
void Octree<T>::insert(T *body) {
    recursively_insert(root, body);
    //recursively_insert_2(body);
}

template<typename T>
void Octree<T>::erase(T *celestial_body) {
    recursively_erase(root, celestial_body);
}

template<typename T>
void Octree<T>::create_space() {
    if (root != nullptr) delete root;
    root = new NodeOctree<T>();
    root->element_octree.size = this->size;
    root->element_octree.centerX = this->centerX;
    root->element_octree.centerY = this->centerY;
    root->element_octree.centerZ = this->centerZ;
}

template<typename T>
void Octree<T>::calc_forces_per_body(T *body) {
    body->set_force(Vec3(0, 0, 0)); //cada fuerza calculada no debe calcularse con la de un tiempo t anterior
    //es decir, no debe ser una acumulación temporal de la fuerza por cuerpo.
    recursively_calc_forces(root, body);
}

template<typename T>
NodeOctree<T> * Octree<T>::locate_node_father(T *body) {
    NodeOctree<T>* father = nullptr; //para no entregar el puntero original
    father = recursively_locate_node_father(root, body);
    return father;
}

template<typename T>
NodeOctree<T> * Octree<T>::locate_body(T *body) {
    NodeOctree<T>* node = nullptr;
    node = recursively_locate_body(root, body);
    return node;
}

template<typename T>
void Octree<T>::query_region(bool (*condition)(NodeOctree<T> *const &,  T *const&),
    void (*action)(T *&, T *&, std::vector<T*>& ),
    T *body, std::vector<T*>& bodies) {
    recursive_query_region(root, condition, action, body, bodies);
}

template<typename T>
void Octree<T>::refresh_theta_value() {
    if (num_bodies < 100) this->theta = 0;
    else if (num_bodies >= 100 and num_bodies < 1000) this-> theta = 0.3;//habria que testar cuantos cuerpos calculados por fuerza bruta
    //puede soportar la computadora de manera fluida
    else if (num_bodies >= 1000 and num_bodies < 10000) this-> theta = 0.5;
    else if (num_bodies >= 10000 and num_bodies < 100000) this-> theta = 0.6;
    else if (num_bodies >= 100000 and num_bodies < 1000000) this-> theta = 0.7;
    else theta = 0.8;
}

template<typename T>
void Octree<T>::refresh_mass_centers() {
    recursive_refresh_mass_centers(root);
}

template<typename T>
void Octree<T>::recursive_refresh_mass_centers(NodeOctree<T> *node) {
    if (!node) return;
    if (node->has_children()) {
        for (auto & i : node->children) {
            recursive_refresh_mass_centers(i);
        }
        node->calc_avg_values();
    }
}

template<typename T>
void Octree<T>::recursive_query_region(NodeOctree<T> *node,
    bool (*condition)(NodeOctree<T> *const &, T*const&),
    void (*action)(T *&, T *&, std::vector<T*>& ),
    T *body, std::vector<T*>& bodies) {

    if (!node or !condition or !action or !body) return;
    if (condition(node, body) == false) return;
    if (node->has_children()) {
        //int index = octant_for_position(body->get_position(), node->element_octree.get_position());
        for (int index = 0; index < 8; index++)
            recursive_query_region(node->children[index], condition, action, body, bodies);
    }
    else {
        for (int i = 0; i < node->element_octree.bodies.size(); i++) {
            if (node->element_octree.bodies[i] and node->element_octree.bodies[i] != body and
            body->get_id() < node->element_octree.bodies[i]->get_id())
                action(node->element_octree.bodies[i], body, bodies);
        }
    }
}

template<typename T>
NodeOctree<T> * Octree<T>::recursively_locate_body(NodeOctree<T> *node, T *body) {
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

template<typename T>
NodeOctree<T> * Octree<T>::recursively_locate_node_father(NodeOctree<T> *node, T *body) {
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

template<typename T>
NodeOctree<T> * Octree<T>::select_child(NodeOctree<T> *node, T *body) {
    Vec3 pos = body->get_position();
    Vec3 center = node->element_octree.get_position();

    uint8_t index = octant_for_position(pos, center);

    return node->children[index];
}

template<typename T>
void Octree<T>::recursively_insert(NodeOctree<T> *&node_octree, T *body) {
    if (!node_octree->has_children()) {//si el nodo es externo...
        if (node_octree->element_octree.bodies.size() == CAPACITY) {//si está lleno
            node_octree->create_children();
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
                //NodeOctree<T> *destinyOld = select_child(node_octree, oldBody);
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
            double new_mass = node_octree->element_octree.mass + body->get_mass();

            node_octree->element_octree.centerOfMassX =
                (node_octree->element_octree.centerOfMassX * node_octree->element_octree.mass
                    + body->get_posX() * body->get_mass())/ new_mass;
            node_octree->element_octree.centerOfMassY =
                (node_octree->element_octree.centerOfMassY * node_octree->element_octree.mass
                    + body->get_posY() * body->get_mass())/ new_mass;
            node_octree->element_octree.centerOfMassZ =
                (node_octree->element_octree.centerOfMassZ * node_octree->element_octree.mass
                    + body->get_posZ() * body->get_mass())/ new_mass;

            node_octree->element_octree.mass = new_mass;
            node_octree->element_octree.bodies.push_back(body);

            ++this->num_bodies;
            //this->num_id++;
            return;
        }
    }
    //NodeOctree<T> *destiny_new = select_child(node_octree, body);
    uint8_t index = octant_for_position(
        body->get_posX(),
        body->get_posY(),
        body->get_posZ(),
        node_octree->element_octree.get_center_x(),
        node_octree->element_octree.get_center_y(),
        node_octree->element_octree.get_center_z());

    recursively_insert(node_octree->children[index], body);
    node_octree->calc_avg_values();
    node_octree->element_octree.bodies.clear();
}

template<typename T>
void Octree<T>::recursively_insert_2(T *&body) {
    //Esta es una función mayormente iterativa que, por simplicidad, se
    //llama recursivamente algunas veces. No es tan eficiente como
    //una recursión pura, pero se evita repetir el bucle de inserción
    //dentro de sí mismo.
    std::vector<NodeOctree<T>*> ancestors;
    NodeOctree<T> *node = root;
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
        NodeOctree<T> *ancestor = ancestors.back();
        ancestor->calc_avg_values();
        ancestors.pop_back();
    }
    ancestors.clear();
}

template<typename T>
bool Octree<T>::recursively_erase(NodeOctree<T> *&node_octree, T *body) {
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
    NodeOctree<T> *destiny = select_child(node_octree, body);
    if (!destiny) return false; //para evitar los nullptr
    bool erased = recursively_erase(destiny, body);
    if (erased) node_octree->calc_avg_values(); //lógico... solo se debería recalcular los
    //datos del nodo padre si es que se ha borrado un cuerpo de sus hijos
    return erased;
}

template<typename T>
void Octree<T>::recursively_calc_forces(const NodeOctree<T> *node_octree, T *body) { //esta funcion va a servir
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

// void Octree<T>::iterative_calc_forces(T *body) { //esta funcion va a servir
//     //para acumular todas las fuerzas de todos los nodos sobre este cuerpo
//
//     //distancia del cuerpo al centro de masa del nodo
//     if (!node_octree || node_octree->element_octree.mass == 0) return;
//
//     const double d = (body->get_position() - node_octree->element_octree.centerOfMass).magnitude();
//     if (d == 0) return; //para evitar una division entre 0
//
//     if (node_octree->element_octree.size/d < theta) { //significa que el nodo está lo suficientemente lejos
//         //y se puede tratar como un solo cuerpo
//         Vec3 new_force = force_exerted_from_to(
//             node_octree->element_octree.mass, node_octree->element_octree.centerOfMass,
//             body->get_mass(), body->get_position()
//             );
//         body->set_force(body->get_force() + new_force);
//     }
//     else if (!node_octree->has_children()) {
//         for (int i = 0; i < node_octree->element_octree.bodies.size(); i++) {
//             if (node_octree->element_octree.bodies[i] != body) {
//                 Vec3 new_force = force_exerted_from_to(
//                 node_octree->element_octree.bodies[i]->get_mass(),node_octree->element_octree.bodies[i]->get_position(), //1
//                 body->get_mass(),body->get_position() //2
//                 );
//                 body->set_force(body->get_force() + new_force);
//             }
//         }
//     }
//     else for (int i=0; i < 8;i++) recursively_calc_forces(node_octree->children[i], body);
// }

template<typename T>
uint8_t Octree<T>::octant_for_position(double posX, double posY, double posZ,
        double centerX, double centerY, double centerZ){
    uint8_t index = 0;
    //se le va agregando bits 1's mientras va cumpliendo las condiciones.
    //Depende directamente de cómo están ordenados los nodos hijos en la función
    //NodeOctree<T>::create_children()
    if (posX >= centerX) index |= 1;
    if (posY >= centerY) index |= 2;
    if (posZ >= centerZ) index |= 4;
    return index;
}