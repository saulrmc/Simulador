//
// Created by Saúl on 8/01/2026.
//

#include "NodeOctree.h"

NodeOctree::NodeOctree() {

}

NodeOctree::~NodeOctree() {
    for (int i = 0; i < 8; i++) delete children[i];
}

void NodeOctree::create_children() {
    for (int i = 0; i < 8; i++) {
        children[i] = new NodeOctree();
        children[i]->element_octree.size = element_octree.size / 2;

        double offset = children[i]->element_octree.size / 2;
        children[i]->element_octree.center = element_octree.center + Vec3(
            (i & 1 ? +offset : -offset),
            (i & 2 ? +offset : -offset),
            (i & 4 ? +offset : -offset)
        );
    }
}

bool NodeOctree::has_children() const {
    return this->children[0] != nullptr;
}

void NodeOctree::calc_avg_values() {
    this->element_octree.mass = 0;
    for (int i = 0; i < 8; i++) {
        if (this->children[i]->element_octree.mass > 0) { //ya que los nodos internos también almacenan masa
            this->element_octree.mass += this->children[i]->element_octree.mass;
        }
    }

    //para el cálculo del centro de masa...
    //rcm = SUM(ri*mi/M);
    //donde:
    //r: vector posicion de la masa de un hijo con respecto al centro del cubo
    //m: masa de un hijo
    //M: masa total de todos los hijos del nodo
    this->element_octree.centerOfMass = Vec3(0,0,0);
    for (int i = 0; i < 8; i++) {
        if (this->children[i]->element_octree.mass > 0) {
            this->element_octree.centerOfMass = this->element_octree.centerOfMass +
                (this->children[i]->element_octree.centerOfMass - this->element_octree.center)*
                    this->children[i]->element_octree.mass / this->element_octree.mass;
        }
    }
}

