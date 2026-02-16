//
// Created by Saúl on 15/02/2026.
//

#include "BrownDwarf.h"

double BrownDwarf::gravitational_biding_energy() {
    return (3.0/(5.0-1.5))*units::G*this->get_mass()*this->get_mass()/this->get_radius();
}
