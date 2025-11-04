//
// Created by Saúl on 4/11/2025.
//

#ifndef SIMULADORGRAVITACIONAL_CUERPOCELESTE_H
#define SIMULADORGRAVITACIONAL_CUERPOCELESTE_H


class CuerpoCeleste {
    public:
    CuerpoCeleste();
    virtual ~CuerpoCeleste();


    private:
    double masa;
    double radio;
    int posX;
    int posY;
    double velX;
    double velY;
};


#endif //SIMULADORGRAVITACIONAL_CUERPOCELESTE_H