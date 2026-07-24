//
// Created by saul on 23/07/26.
//

#include "NodeOctree.h"

inline NodeOctree::~NodeOctree() {
    for (int i = 0; i < 8; i++) delete children[i];
    //children.clear();
}


inline void NodeOctree::create_children() {
    for (int i = 0; i < 8; i++) {
        children[i] = new NodeOctree();
        children[i]->element_octree.size = element_octree.size / 2;
        //de esta manera se le suma +- 1/4 de la longitud de un lado del nodo padre
        //al centro del nodo padre en todos los ejes y con eso se determina
        //las posiciones de los centros de los hijos.
        //Con respecto al offset...tranquilamente pudo haber sido un
        //if para cada número del 0 al 7 pero creo que queda mejor como está ahora.
        double offset = children[i]->element_octree.size / 2;
        // children[i]->element_octree.center = element_octree.center + Vec3(
        //     (i & 1 ? +offset : -offset),
        //     (i & 2 ? +offset : -offset),
        //     (i & 4 ? +offset : -offset)
        // );

        children[i]->element_octree.center.set_x(element_octree.center.get_x() +
            (i & 1 ? +offset : -offset));
        children[i]->element_octree.center.set_y(element_octree.center.get_y() +
            (i & 2 ? +offset : -offset));
        children[i]->element_octree.center.set_z( element_octree.center.get_z() +
            (i & 4 ? +offset : -offset));
        //children[i]->calc_frontier_values();
    }
}
inline void NodeOctree::calc_avg_values() {
    this->element_octree.mass = 0;
    for (int i = 0; i < 8; i++) {
        if (this->children[i]->element_octree.mass > 0) { //ya que los nodos internos también almacenan masa
            this->element_octree.mass += this->children[i]->element_octree.mass;
        }
    }

    //para el cálculo del centro de masa...
    //rcm = SUM(ri*mi/M);
    //donde:
    //r: vector posicion de la masa de un hijo
    //m: masa de un hijo
    //M: masa total de todos los hijos del nodo
    this->element_octree.centerOfMass = Vec3(0, 0, 0);

    for (int i = 0; i < 8; i++) {
        if (this->children[i]->element_octree.mass > 0) {
            this->element_octree.centerOfMass = this->element_octree.centerOfMass +
                (this->children[i]->element_octree.centerOfMass)* //como aquí antes le restaba el
                    //centro del nodo eso provocaba que la poda de nodos en la profundidad de acceso
                        //al octree sea muy temprana e incorrecta. Esto hacía al programa más rápido de lo que debía
                    this->children[i]->element_octree.mass / this->element_octree.mass;
        }
    }
}