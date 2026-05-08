//
// Created by Saúl on 8/01/2026.
//
#ifndef SIMULADORGRAVITACIONAL_OCTREE_H
#define SIMULADORGRAVITACIONAL_OCTREE_H
#include <cstdint>

#include "NodeOctree.h"
#include "../Physics.h"
#include <vector>
template<typename T>
class Octree {
public:
    double get_size() const;

    void set_size(const double size);

    Vec3 get_center() const;

    void set_center(const Vec3 &center);

    //getters temporales (tal vez) para testear
    int get_num_bodies() const;
    double get_theta() const;

    Octree();
    virtual ~Octree();
    void refresh_theta_value();
    void refresh_mass_centers();
    
    void insert(T *);
    void erase(T *);
    void create_space();
    void calc_forces_per_body(T *);
    NodeOctree<T>* locate_node_father(T *);
    NodeOctree<T>* locate_body(T *);
    void query_region(bool (*condition)(NodeOctree<T> *const &, T*const&),
        void (*action)(T *&, T *&,
        std::vector<T*>&), T *body,  std::vector<T*>&);
    private:
    NodeOctree<T>* root;
    NodeOctree<T>* recursively_locate_node_father(NodeOctree<T>* node, T *);
    NodeOctree<T>* select_child(NodeOctree<T>* node, T *);
    NodeOctree<T>* recursively_locate_body(NodeOctree<T> *node, T *);
    void recursively_insert(NodeOctree<T>*&, T *);
    void recursively_insert_2(T *&);
    bool recursively_erase(NodeOctree<T>*&, T *);
    void recursively_calc_forces(const NodeOctree<T> *node, T *);
    uint8_t octant_for_position(const Vec3 &pos, const Vec3& center);
    void recursive_query_region(NodeOctree<T> *node,
    bool (*condition)(NodeOctree<T> *const &, T*const&),
    void (*action)(T *&, T *&, std::vector<T*>& ),
    T *body, std::vector<T*>& bodies);
    void recursive_refresh_mass_centers(NodeOctree<T> *node);
    //por las pruebas que realicé es mejor dejar en 0 el valor de THETA cuando hay pocos cuerpos
    //porque de lo contrario añade error pero tampoco puede quedarse en 0 porque sino no tiene sentido aplicar un
    //octree si es que los cálculos van a ser por fuerza bruta
    int num_bodies;
    double theta = 0;
    //int num_id;
    double size;
    Vec3 center;

};
#include "Octree.tpp"
#endif //SIMULADORGRAVITACIONAL_OCTREE_H
