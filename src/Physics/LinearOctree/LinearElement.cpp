//
// Created by saul on 1/05/26.
//

#include "LinearElement.h"

template<typename T>
LinearElement<T>::LinearElement() {
    object = nullptr;
}

template<typename T>
LinearElement<T>::~LinearElement() {

}

template<typename T>
double LinearElement<T>::get_object_posX() {
    return object->get_posX();
}

template<typename T>
double LinearElement<T>::get_object_posY() {
    return object->get_posY();
}

template<typename T>
double LinearElement<T>::get_object_posZ() {
    return object->get_posZ();
}

template<typename T>
void LinearElement<T>::set_object_posX(const double object_posX) {
    object->set_posX(object_posX);
}

template<typename T>
void LinearElement<T>::set_object_posY(const double object_posY) {
    object->set_posY(object_posY);
}

template<typename T>
void LinearElement<T>::set_object_posZ(const double object_posZ) {
    object->set_posZ(object_posZ);
}

template<typename T>
unsigned int LinearElement<T>::get_morton_code() const {
    return mortonCode;
}
template<typename T>
void LinearElement<T>::set_morton_code(const unsigned int morton_code) {
    mortonCode = morton_code;
}