//
// Created by SILVER on 21/12/2025.
//

#ifndef SIMULADORGRAVITACIONAL_MOTORFISICOUTILS_H
#define SIMULADORGRAVITACIONAL_MOTORFISICOUTILS_H
#include <math.h>
#include <vector>
namespace unidades {
    //sistema de unidades
    constexpr double MASA_0      = 5.97217e24; //masa de la Tierra en kg xd
    constexpr double DISTANCIA_0 = 3.844e8; //distancia real de la Tierra a la Luna en metros
    constexpr double TIEMPO_0    = 8.64e4; //1 día en segundos
    constexpr double G           = 0.0524; //constante G de la ley de gravitación universal del
                                              //nuevo sistema de unidades, sin redondear es 0.052386
}


#endif //SIMULADORGRAVITACIONAL_MOTORFISICOUTILS_H