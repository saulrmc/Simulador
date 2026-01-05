//
// Created by Saúl on 21/12/2025.
//

#ifndef SIMULADORGRAVITACIONAL_CUERPOCELESTE_H
#define SIMULADORGRAVITACIONAL_CUERPOCELESTE_H
#include <string>

class CuerpoCeleste {

public:
    CuerpoCeleste();
    virtual ~CuerpoCeleste();
    CuerpoCeleste(double, double, double, double, double);
    CuerpoCeleste(std::string, double, double, double, double, double);

    double get_pos_x() const;
    void set_pos_x(const double pos_x);
    double get_pos_y() const;
    void set_pos_y(const double pos_y);
    double get_vel_x() const;
    void set_vel_x(const double vel_x);
    double get_vel_y() const;
    void set_vel_y(const double vel_y);
    double get_masa() const;
    void set_masa(const double masa);
    double get_radio() const;
    void set_radio(const double radio);
    double get_pos_z() const;
    void set_pos_z(const double pos_z);
    double get_vel_z() const;
    void set_vel_z(const double vel_z);
    std::string get_nombre() const;
    void set_nombre(const std::string &nombre);

    //para el caso de las colisiones
    void absorberInsignificante(const CuerpoCeleste &);
    bool esInsignificante(const CuerpoCeleste &);

private:
    std::string nombre;

    double posX;
    double posY;
    double posZ;

    double velX;
    double velY;
    double velZ;

    double masa;
    double radio;

    //pero qué ocurre cuando un objeto enorme colisiona con uno muy pequeño?
    //como en el caso del Sol con la Luna.
    //En la vida real una colisión así mantendría al Sol practicamente igual
    //pero ahora qué pasaría si lanzamos un millón de Lunas al Sol una por una?
    //individualmente no hacen mucho pero llega un momento en el que ya no se puede
    //despreciar, por ejemplo, sus masas.
    double velXAcumulativo;
    double velYAcumulativo;
    double velZAcumulativo;

    double masaAcumulativo;
    double radioAcumulativo;

};


#endif //SIMULADORGRAVITACIONAL_CUERPOCELESTE_H