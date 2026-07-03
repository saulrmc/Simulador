//
// Created by saul on 10/06/26.
//

#include "SpatialStructure.h"

template<typename Structure, typename T>
void SpatialStructure<Structure, T>::insert(Vec3& position, double mass, int indexBody) {
    static_cast<Structure*>(this)->insertImpl(position, mass, indexBody);
}