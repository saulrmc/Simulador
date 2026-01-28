//
// Created by Saúl on 21/12/2025.
//

#ifndef SIMULADORGRAVITACIONAL_PHYSICSUNITS_H
#define SIMULADORGRAVITACIONAL_PHYSICSUNITS_H
#include <cmath>
namespace units {
    //sistema de unidades
    constexpr double MASS_0      = 1.0; //masa de la Tierra
    constexpr double DISTANCE_0  = 1.0; //distancia real de la Tierra a la Luna
    constexpr double TIME_0      = 1.0; //1 día
    constexpr double G           = 0.0524; //constante G de la ley de gravitación universal del
                                              //nuevo sistema de unidades, sin redondear es 0.052386
    constexpr double DELTA_TIME = 0.001;   // 0.001 días
    constexpr double HALF_DELTA_TIME = 0.5 * DELTA_TIME;

}


#endif //SIMULADORGRAVITACIONAL_PHYSICSUNITS_H