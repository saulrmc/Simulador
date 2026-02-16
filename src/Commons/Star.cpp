//
// Created by Saúl on 15/02/2026.
//

#include "Star.h"

double Star::gravitational_biding_energy() {
    return (3.0/(5.0-3.0))*units::G*this->get_mass()*this->get_mass()/this->get_radius();
}
