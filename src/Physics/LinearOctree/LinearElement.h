//
// Created by saul on 1/05/26.
//

#ifndef SIMULADORGRAVITACIONAL_LINEARELEMENT_H
#define SIMULADORGRAVITACIONAL_LINEARELEMENT_H
#include "../../Commons/CelestialBody.h"


class LinearElement {
    public:
    LinearElement();
    virtual ~LinearElement();
    private:
    CelestialBody* body;
};



#endif //SIMULADORGRAVITACIONAL_LINEARELEMENT_H
