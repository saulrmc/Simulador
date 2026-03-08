//
// Created by Saúl on 15/02/2026.
//

#ifndef SIMULADORGRAVITACIONAL_BROWNDWARF_H
#define SIMULADORGRAVITACIONAL_BROWNDWARF_H
#include "CelestialBody.h"


class BrownDwarf : public CelestialBody{
    public:
    BrownDwarf();
    virtual ~BrownDwarf();
    private:
    double gravitational_biding_energy() override;
};


#endif //SIMULADORGRAVITACIONAL_BROWNDWARF_H