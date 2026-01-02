//
// Created by SILVER on 21/12/2025.
//

#include "CuerpoCeleste.h"

CuerpoCeleste::CuerpoCeleste() {
    velX=0;
    velY=0;
    posX=0;
    posY=0;
    masa=0;
    radio=0;
}

CuerpoCeleste::~CuerpoCeleste() {
}

CuerpoCeleste::CuerpoCeleste(double velX, double velY, double posX,
    double posY, double masa, double radio) {
    this->velX = velX;
    this->velY = velY;
    this->posX = posX;
    this->posY = posY;
    this->masa = masa;
    this->radio = radio;
}

double CuerpoCeleste::get_pos_x() const {
    return posX;
}

void CuerpoCeleste::set_pos_x(const double pos_x) {
    posX = pos_x;
}

double CuerpoCeleste::get_pos_y() const {
    return posY;
}

void CuerpoCeleste::set_pos_y(const double pos_y) {
    posY = pos_y;
}

double CuerpoCeleste::get_vel_x() const {
    return velX;
}

void CuerpoCeleste::set_vel_x(const double vel_x) {
    velX = vel_x;
}

double CuerpoCeleste::get_vel_y() const {
    return velY;
}

void CuerpoCeleste::set_vel_y(const double vel_y) {
    velY = vel_y;
}

double CuerpoCeleste::get_masa() const {
    return masa;
}

void CuerpoCeleste::set_masa(const double masa) {
    this->masa = masa;
}

double CuerpoCeleste::get_radio() const {
    return radio;
}

void CuerpoCeleste::set_radio(const double radio) {
    this->radio = radio;
}

void CuerpoCeleste::absorberInsignificante(CuerpoCeleste cuerpo) {
    this->velXAcumulativo += cuerpo.velX;
    this->velYAcumulativo += cuerpo.velY;
    this->masaAcumulativo += cuerpo.masa;
}

double CuerpoCeleste::get_pos_z() const {
    return posZ;
}

void CuerpoCeleste::set_pos_z(const double pos_z) {
    posZ = pos_z;
}

double CuerpoCeleste::get_vel_z() const {
    return velZ;
}

void CuerpoCeleste::set_vel_z(const double vel_z) {
    velZ = vel_z;
}


