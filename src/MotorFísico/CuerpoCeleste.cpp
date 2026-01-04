//
// Created by Saúl on 21/12/2025.
//

#include "CuerpoCeleste.h"

CuerpoCeleste::CuerpoCeleste() {
    velX=0;
    velY=0;
    velZ=0;
    posX=0;
    posY=0;
    posZ=0;
    masa=0;
    radio=0;


    masaAcumulativo=0;
    radioAcumulativo=0;
    velXAcumulativo=0;
    velYAcumulativo=0;
    velZAcumulativo=0;

}

CuerpoCeleste::~CuerpoCeleste() {
}

CuerpoCeleste::CuerpoCeleste(double posX,
    double posY, double posZ, double masa, double radio) {
    this->velX = 0;
    this->velY = 0;
    this->velZ = 0;
    this->posX = posX;
    this->posY = posY;
    this->posZ = posZ;
    this->masa = masa;
    this->radio = radio;

    masaAcumulativo=0;
    radioAcumulativo=0;
    velXAcumulativo=0;
    velYAcumulativo=0;
    velZAcumulativo=0;
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
//

void CuerpoCeleste::absorberInsignificante(const CuerpoCeleste &cuerpo) {
    this->velXAcumulativo += cuerpo.velX;
    this->velYAcumulativo += cuerpo.velY;
    this->masaAcumulativo += cuerpo.masa;
}

bool CuerpoCeleste::esInsignificante(const CuerpoCeleste &cuerpo) {

    if (this->masaAcumulativo + this->masa == this->masa) return true;
    return false;
}