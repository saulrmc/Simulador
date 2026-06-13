//
// Created by Saúl on 8/01/2026.
//

#ifndef SIMULADORGRAVITACIONAL_NODEOCTREE_H
#define SIMULADORGRAVITACIONAL_NODEOCTREE_H
#include "ElementOctree.h"
class NodeOctree {
public:
    NodeOctree();
    virtual ~NodeOctree();
    bool has_children() const;
    void calc_avg_values();
    const NodeOctree* n_child(int index) const;
    friend class Octree;
    //exclusivo de este problema (osea del simulador)
    Vec3 get_node_center() const;
    double get_node_size() const;
    // double get_frontier_x_min() const;
    // double get_frontier_y_min() const;
    // double get_frontier_z_min() const;
    // double get_frontier_x_max() const;
    // double get_frontier_y_max() const;
    // double get_frontier_z_max() const;
    // void calc_frontier_values();

private:
    void create_children();

    ElementOctree element_octree;
    NodeOctree *children[8]{};
    //std::vector<NodeOctree*> children;

    //el index 0 y 1 para xMin y xMax
    //el index 2 y 3 para yMin & yMax
    //el index 4 y 5 para zMin y zMax
    // double frontierValue[6]{};
};


inline NodeOctree::NodeOctree() {
    ElementOctree();
}


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


inline bool NodeOctree::has_children() const {
    return this->children[0] != nullptr;
    //return !children.empty();
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

inline const NodeOctree * NodeOctree::n_child(int index) const {
    if (!this->has_children())return nullptr;
    NodeOctree * n_child = children[index];
    return n_child;
}


inline Vec3 NodeOctree::get_node_center() const {
    return this->element_octree.center;
}


inline double NodeOctree::get_node_size() const {
    return this->element_octree.size;
}
//
// 
// double NodeOctree::get_frontier_x_min() const {
//     return frontierValue[0];
// }
//
// 
// double NodeOctree::get_frontier_y_min() const {
//     return frontierValue[2];
// }
//
// 
// double NodeOctree::get_frontier_z_min() const {
//     return frontierValue[4];
// }
//
// 
// double NodeOctree::get_frontier_x_max() const {
//     return frontierValue[1];
// }
//
// 
// double NodeOctree::get_frontier_y_max() const {
//     return frontierValue[3];
// }
//
// 
// double NodeOctree::get_frontier_z_max() const {
//     return frontierValue[5];
// }
//
// 
// void NodeOctree::calc_frontier_values() {
//     double half_size = this->element_octree.size/2;
//     //minX
//     frontierValue[0] = this->element_octree.centerX - half_size;
//     //minY
//     frontierValue[2] = this->element_octree.centerY - half_size;
//     //minZ
//     frontierValue[4] = this->element_octree.centerZ - half_size;
//     //maxX
//     frontierValue[1] = this->element_octree.centerX + half_size;
//     //maxY
//     frontierValue[3] = this->element_octree.centerY + half_size;
//     //maxZ
//     frontierValue[5] = this->element_octree.centerZ + half_size;
// }
#endif //SIMULADORGRAVITACIONAL_NODEOCTREE_H
