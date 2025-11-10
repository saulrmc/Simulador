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
    double radio; //suponiendo que todos los cuerpos son
    //redondos pero eso no pasa en la vida real xd
    int posX;
    int posY;
    int posZ;
    double velX;
    double velY;
    double velZ;
};


#endif //SIMULADORGRAVITACIONAL_CUERPOCELESTE_H