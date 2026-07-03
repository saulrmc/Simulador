//
// Created by saul on 10/06/26.
//

#ifndef SIMULADORGRAVITACIONAL_SPATIALSTRUCTURE_H
#define SIMULADORGRAVITACIONAL_SPATIALSTRUCTURE_H
#include "Vec3.h"

template <typename Structure, typename T>
class SpatialStructure {
    public:
    void insert(Vec3& position, double mass, int indexBody);

};

#endif //SIMULADORGRAVITACIONAL_SPATIALSTRUCTURE_H
