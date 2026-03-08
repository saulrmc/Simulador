//
// Created by Saúl on 15/02/2026.
//

#ifndef SIMULADORGRAVITACIONAL_STAR_H
#define SIMULADORGRAVITACIONAL_STAR_H
#include "CelestialBody.h"


class Star : public CelestialBody{//por ahora no se hará la distinción entre
    //los distintos tipos de estrellas según clasificación de harvard
    public:
    Star();
    virtual ~Star();
    double gravitational_biding_energy() override;
    private:

};


#endif //SIMULADORGRAVITACIONAL_STAR_H