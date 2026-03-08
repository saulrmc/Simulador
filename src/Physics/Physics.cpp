//
// Created by Saúl on 8/01/2026.
//

#include "Physics.h"
static constexpr double EPSILON = 1e-5;


//cuando se usen estas funciones se debe manejar correctamente el uso de dt y dt/2
//para usar leapfrog kdk o leapfrog puro

Vec3 force_exerted_from_to(const double m1, const Vec3& position1,
    const double m2, const Vec3& position2) {
    //F=G*m1*m2/r^2
    const Vec3 r = position2 - position1;
    const double dist = r.magnitude() + EPSILON;
    return r*(-units::G*(m1*m2)/(dist*dist*dist));
}

Vec3 next_velocity_for_delta_time(const double deltaTime, const double m,
    const Vec3& force, const Vec3& current_v) { //delta time debería ser dt/2 pero eso no le corresponde a esta funcion
    //v(t + dt/2) = v(t) + (F(t)/m) * dt
    return Vec3(current_v + (force/m)*deltaTime);
}

Vec3 next_position_for_delta_time(const double deltaTime, const Vec3& velocity, const Vec3& current_x) {
    //x(t + dt) = x(t) + v(t + dt/2) * dt
    return Vec3(current_x + velocity*deltaTime);
}


double specific_impact_energy(const double mass1, const double mass2, const Vec3 &relativeVelocity) {
    //se obtiene dividiendo la energía total de los dos cuerpos cambiada al sistema del centro de masa por
    //la masa total de dicho sistema
    const double reducedMass = mass1*mass2/(mass1+mass2);
    return 1.0/2*reducedMass*relativeVelocity.dot(relativeVelocity)/(mass1 + mass2);
}

double effective_specific_impact_energy(const double mass1, const double mass2, const Vec3 &vel1, const Vec3 &vel2,
    const Vec3& center1, const Vec3& center2) {
    //aunque en realidad en cálculo debería ser con las partes de las masas que van a interactuar pero... :/
    const Vec3 relativeVelocity = vel1-vel2;
    const Vec3 distance = center1-center2;
    double energy = specific_impact_energy(mass1, mass2, relativeVelocity);
    double cos_angle = relativeVelocity.dot(distance)/(relativeVelocity.magnitude()*distance.magnitude());
    return energy*cos_angle*cos_angle;
}

double mutual_escape_velocity(double mass1, double mass2, Vec3 pos1, Vec3 pos2) {
    double distance = (pos2 - pos1).magnitude();
    return sqrt(2*units::G*(mass1 + mass2)/distance);
}

double catastrophic_disruption_criterion(Vec3 &vel1, Vec3 &vel2, double radius1, double radius2) {
    // se asume que ell régimen de fuerza es despreciable para satélites, planetas y demás cuerpos más masivos...

    const Vec3 relativeVelocity = vel1-vel2;
    //POR AHORA se va a asumir que los cuerpos son de densidades iguales
    const double combinedRadius = pow(radius1*radius1*radius1 + radius2*radius2*radius2, 1/3);
    const double q_g = 1; //coeficiente adimensional con valor cercano a 1
    const double density = 1000; //densidad arbitraria entre los dos cuerpos
    const double mi = 1/2; //medida de cómo la energía y el momentum del proyectil se acopla al objetivo
    //el valor está entre 1/3 y 2/3. 1/3 es para momentum puro y 2/3 para energía pura

    return q_g * pow(density * units::G, 3*mi/2) * pow(combinedRadius, 3*mi)
    * pow(relativeVelocity.magnitude(), 2 - 3*mi);
}

double mass_largest_remmant(double mass1, double mass2, double impactEnergy, double catastrophicDisCrit) {
    return (1 - 0.5 * (impactEnergy/catastrophicDisCrit)) * (mass1 + mass2);
}