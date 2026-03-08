//
// Created by Saúl on 15/02/2026.
//

#ifndef SIMULADORGRAVITACIONAL_PLANET_H
#define SIMULADORGRAVITACIONAL_PLANET_H
#include "CelestialBody.h"


class Planet : public CelestialBody{ //por ahora no se hará distinción entre planetas
    //enanos, rocosos y gaseosos
    public:
    Planet();
    virtual ~Planet();
    double gravitational_biding_energy() override;
    private:


};


#endif //SIMULADORGRAVITACIONAL_PLANET_H