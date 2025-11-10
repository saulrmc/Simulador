//
// Created by Saúl on 4/11/2025.
//

#ifndef SIMULADORGRAVITACIONAL_CUERPOCELESTE_H
#define SIMULADORGRAVITACIONAL_CUERPOCELESTE_H
#include "Utils.h"

class CuerpoCeleste {
public:

    CuerpoCeleste();
    virtual ~CuerpoCeleste();

    double get_masa() const;
    void set_masa(const double masa);
    double get_radio() const;
    void set_radio(const double radio);
    int get_pos_x() const;
    void set_pos_x(const int pos_x);
    int get_pos_y() const;
    void set_pos_y(const int pos_y);
    double get_vel_x() const;
    void set_vel_x(const double vel_x);
    double get_vel_y() const;
    void set_vel_y(const double vel_y);
    string get_nombre() const;
    void set_nombre(const string &nombre);


    private:
    double masa;
    double radio; //suponiendo que todos los cuerpos son
    //redondos pero eso no pasa en la vida real xd
    int posX;
    int posY;
    //int posZ; xd
    double velX;
    double velY;
    //double velZ; xd
    string nombre;
};


#endif //SIMULADORGRAVITACIONAL_CUERPOCELESTE_H