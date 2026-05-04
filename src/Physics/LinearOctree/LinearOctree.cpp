//
// Created by saul on 1/05/26.
//

#include "LinearOctree.h"
template<typename T>
LinearOctree<T>::LinearOctree() {
}

template<typename T>
LinearOctree<T>::~LinearOctree() {
    elements.clear();
}

template<typename T>
double LinearOctree<T>::get_center_pos_x() const {
    return centerPosX;
}

template<typename T>
void LinearOctree<T>::set_center_pos_x(double centerPosX) {
    this->centerPosX = centerPosX;
    set_minPosX();
}

template<typename T>
double LinearOctree<T>::get_center_pos_y() const {
    return centerPosY;
}

template<typename T>
void LinearOctree<T>::set_center_pos_y(double centerPosY) {
    this->centerPosY = centerPosY;
    set_minPosY();
}

template<typename T>
double LinearOctree<T>::get_center_pos_z() const {
    return centerPosZ;
}

template<typename T>
void LinearOctree<T>::set_center_pos_z(double centerPosZ) {
    this->centerPosZ = centerPosZ;
    set_minPosZ();
}

template<typename T>
bool LinearOctree<T>::refresh_all_morton_codes() {
    bool outOfSpace = false;
    for (LinearElement<T> &element : elements) {
        if (!calculate_morton_code(element)) outOfSpace = true;
    }
    return outOfSpace;
}

template<typename T>
void LinearOctree<T>::insert(T) {
    LinearElement<T> node = new LinearElement<T>();

}

template<typename T>
int LinearOctree<T>::get_integer_posN(double posN, double minPosN) const {
    int number = (posN - minPosN)/resolution;
    return number > 0 ? number : -1;
}

template<typename T>
bool LinearOctree<T>::calculate_morton_code(LinearElement<T> &element) const {
    int x = get_integer_posN(element.get_object_posX(), minPosX);
    int y = get_integer_posN(element.get_object_posY(), minPosY);
    int z = get_integer_posN(element.get_object_posZ(), minPosZ);
    if (x < 0 or y < 0 or z < 0) return false;
    element.set_morton_code(
            expand_bits_64(static_cast<unsigned long long int>(x))      |
            expand_bits_64(static_cast<unsigned long long int>(y)) << 1 |
            expand_bits_64(static_cast<unsigned long long int>(z)) << 2
            );
    return true;
}

template<typename T>
void LinearOctree<T>::set_minPosX() {
    minPosX = centerPosX - size/2;
}

template<typename T>
void LinearOctree<T>::set_minPosY() {
    minPosY = centerPosY - size/2;
}

template<typename T>
void LinearOctree<T>::set_minPosZ() {
    minPosZ = centerPosZ - size/2;
}

template<typename T>
unsigned long long int LinearOctree<T>::expand_bits_64(unsigned long long int number) const {
    //El objetivo es separar todos los bits del número dejando 2
    //espacios en 0 entre cada bit.
    number &= 0x1FFFFF; //Solo se consideran 21 bits
    number = (number | (number << 32)) & 0x001F00000000FFFF;
    number = (number | (number << 16)) & 0x001F0000FF0000FF;
    number = (number | (number << 8))  & 0x100F00F00F00F00F;
    number = (number | (number << 4))  & 0x10C30C30C30C30C3;
    number = (number | (number << 2))  & 0x1249249249249249;
    return number;
}

