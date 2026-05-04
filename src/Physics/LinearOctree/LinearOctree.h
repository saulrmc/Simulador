//
// Created by saul on 1/05/26.
//

#ifndef SIMULADORGRAVITACIONAL_LINEAROCTREE_H
#define SIMULADORGRAVITACIONAL_LINEAROCTREE_H
#include "LinearElement.h"
#include <vector>

template<typename T>
class LinearOctree {
public:
    LinearOctree();
    virtual ~LinearOctree();
    double get_center_pos_x() const;
    void set_center_pos_x(double centerPosX);
    double get_center_pos_y() const;
    void set_center_pos_y(double centerPosY);
    double get_center_pos_z() const;
    void set_center_pos_z(double centerPosZ);
    bool refresh_all_morton_codes();

    void insert(T);

    private:
    std::vector<LinearElement<T>> elements{};
    int get_integer_posN(double posN, double minPosN) const;
    bool calculate_morton_code(LinearElement<T> &element) const;
    void set_minPosX();
    void set_minPosY();
    void set_minPosZ();
    unsigned long long int expand_bits_64(unsigned long long int number) const;
    double size;
    double centerPosX;
    double centerPosY;
    double centerPosZ;
    double minPosX;
    double minPosY;
    double minPosZ;
    //Indica la precisión espacial que tendrá la cuadrícula.
    //A menor resolución mayor cantidad de cuadrículas.
    //Se recomienda establecer una resolución igual a 1
    //entre una potencia de 2. Esto es:
    //resolution: 1/2^n
    double resolution;
};

#endif //SIMULADORGRAVITACIONAL_LINEAROCTREE_H
