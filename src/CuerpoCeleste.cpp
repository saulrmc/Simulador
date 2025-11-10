//
// Created by Saúl on 4/11/2025.
//

#include "CuerpoCeleste.h"

CuerpoCeleste::CuerpoCeleste() {
    masa=0;
    radio=0;
    posX=0;
    posY=0;
    nombre= "";
    velX=0;
    velY=0;
}

CuerpoCeleste::~CuerpoCeleste() {
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

int CuerpoCeleste::get_pos_x() const {
    return posX;
}

void CuerpoCeleste::set_pos_x(const int pos_x) {
    posX = pos_x;
}

int CuerpoCeleste::get_pos_y() const {
    return posY;
}

void CuerpoCeleste::set_pos_y(const int pos_y) {
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

string CuerpoCeleste::get_nombre() const {
    return nombre;
}

void CuerpoCeleste::set_nombre(const string &nombre) {
    this->nombre = nombre;
}

