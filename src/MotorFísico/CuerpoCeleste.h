//
// Created by SILVER on 21/12/2025.
//

#ifndef SIMULADORGRAVITACIONAL_CUERPOCELESTE_H
#define SIMULADORGRAVITACIONAL_CUERPOCELESTE_H
#include <cstdint>


class CuerpoCeleste {
    public:
    CuerpoCeleste();
    virtual ~CuerpoCeleste();
    private:
    uint16_t posX;
    uint16_t posY;
    double velX;
    double velY;
};


#endif //SIMULADORGRAVITACIONAL_CUERPOCELESTE_H