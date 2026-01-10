//
// Created by Saúl on 8/01/2026.
//

#include "Octree.h"

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


void Octree::recursively_insert(NodeOctree *&node_octree, CelestialBody *body) {
    if (!node_octree) {
        node_octree = new NodeOctree;
        node_octree->element_octree.body = body;
        return;
    }
    if (!node_octree->element_octree.body) {//si el nodo no contiene ningun cuerpo entonces se le
        //coloca uno
        node_octree->element_octree.body = body;
    }
    if (node_octree->children[0] != nullptr) { //osea es un nodo interno

    }
    else {//nodo externo
        if (node_octree->element_octree.body) {//si el nodo ya contiene un cuerpo...
            //entonces se debe crear hijos y reubicar los cuerpos
            node_octree->create_children();

        }
    }

}

void Octree::recursively_erase(NodeOctree *&node_octree, CelestialBody *body) {
}

