//
// Created by Saúl on 8/01/2026.
//

#include "Physics.h"
static constexpr double EPSILON = 1e-5;
static constexpr double DENSITY = 9510.77; //una densidad de 1000 kg/m^3 en el sistema de unidades del simulador
static constexpr double MI = 0.5; //Medida de cómo la energía y el momentum del proyectil se acopla al objetivo
//el valor está entre 1/3 y 2/3. 1/3 es para momentum puro y 2/3 para energía pura. En el paper de donde
//obtuve este número se representa como el símbolo "mi" griego pero un sombrero encima

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


// double specific_impact_energy(const double mass1, const double mass2, const Vec3 &relativeVelocity) {
//     //se obtiene dividiendo la energía total de los dos cuerpos cambiada al sistema del centro de masa por
//     //la masa total de dicho sistema
//     const double reducedMass = mass1*mass2/(mass1+mass2);
//     return 1.0/2*reducedMass*relativeVelocity.dot(relativeVelocity)/(mass1 + mass2);
// }

// double effective_specific_impact_energy(const double mass1, const double mass2, const Vec3 &vel1, const Vec3 &vel2,
//     const Vec3& center1, const Vec3& center2) {
//     //aunque en realidad en cálculo debería ser con las partes de las masas que van a interactuar pero... :/
//     const Vec3 relativeVelocity = vel1-vel2;
//     const Vec3 distance = center1-center2;
//     double energy = specific_impact_energy(mass1, mass2, relativeVelocity);
//     double cos_angle = relativeVelocity.dot(distance)/(relativeVelocity.magnitude()*distance.magnitude());
//     return energy*cos_angle*cos_angle;
// }

double mutual_escape_velocity(double mass1, double mass2, Vec3 pos1, Vec3 pos2) {
    double distance = (pos2 - pos1).magnitude();
    return sqrt(2.0*units::G*(mass1 + mass2)/distance);
}

double mutual_escape_velocity_mod(double largestMass, double massInteract) {
    const double R = pow(3*(largestMass + massInteract)/(4 * std::numbers::pi * DENSITY), 1.0/3);
    return sqrt(2*units::G*(largestMass + massInteract)/R);
}

double catastrophic_disruption_criterion(const Vec3 &vel1, const Vec3 &vel2, double combinedRadius) {
    // se asume que el régimen de fuerza es despreciable para satélites, planetas y demás cuerpos más masivos...

    const Vec3 relativeVelocity = vel1-vel2;
    //POR AHORA se va a asumir que los cuerpos son de densidades iguales
    const double q_g = 1; //coeficiente adimensional con valor cercano a 1
    const double density = DENSITY; //densidad arbitraria entre los dos cuerpos
    const double mi = MI;

    return q_g * pow(density * units::G, 3.0*mi/2) * pow(combinedRadius, 3*mi)
    * pow(relativeVelocity.magnitude(), 2 - 3*mi);
}

double collision_timescale(double radius1, double radius2, double distance, double relativeVelocity) {
    return (radius1 + radius2 - distance) / std::max(relativeVelocity, 1e-8);
}

double collision_angle(const Vec3 &vel1, const Vec3 &vel2, const Vec3& center1, const Vec3& center2) {
    const Vec3 relativeVelocity = vel1 - vel2;
    const Vec3 distance = center1-center2;
    double cos_angle = relativeVelocity.dot(distance)/(relativeVelocity.magnitude()*distance.magnitude());
    return acos(cos_angle);
}

double mass_interact(const Vec3 &vel1, const Vec3 &vel2, const Vec3& center1, const Vec3& center2, double radius2) {
    double collisionAngle = collision_angle(vel1, vel2, center1, center2);
    double l_parameter = (center1 - center2).magnitude() * (1 - sin(collisionAngle));
    return DENSITY * std::numbers::pi * l_parameter*l_parameter * (radius2 - l_parameter/3.0);
}

double disruption_curve(double radius1, double radius2) {
    const double combinedRadius = pow(radius1*radius1*radius1 + radius2*radius2*radius2, 1.0/3);
    double c = 1.9; //representa la diferencia entre la energía de
        //enlace gravitacional y el criterio de disipación para masas iguales. El valor recomendado
        //para cuerpos como planetas es de 1.9+-0.3
    return c * (4.0/5) * std::numbers::pi * DENSITY * units::G * combinedRadius * combinedRadius;
}

double critical_impact_velocity_mod(double combinedRadius) {
    double c = 1.9;
    return sqrt(32.0 * std::numbers::pi * c/5) * sqrt(DENSITY * units::G) * combinedRadius;
}

double disruption_criterion(double disruptionCurve, double relationMass) {
    double exp = 2.0/(3*MI) - 1;
    double secondTerm = pow(0.25*(relationMass + 1)*
        (relationMass + 1)/relationMass, exp);
    return disruptionCurve*secondTerm;
}

double critical_impact_velocity(double criticalImpVelMod, double relationMass) {
    double exp = 1.0/(3*MI);
    double firstTerm = pow(0.25*(relationMass + 1) *
        (relationMass + 1)/relationMass,exp);
    return firstTerm * criticalImpVelMod;
}

double disruption_energy_by_angle(double disruptionCriterion, double reducedMass, double reducedMassMod) {
    double exp = 2 - 3.0*MI/2;
    double firstTerm = pow(reducedMass/reducedMassMod,exp);
    return firstTerm * disruptionCriterion;
}

double critical_impact_velocity_by_angle(double disruptionEnergy, double totalMass, double reducedMass) {
    //así lo llama en el paper pero no usa un ángulo
   return sqrt(2.0*disruptionEnergy*totalMass/reducedMass);
}

double specific_impact_energy(double massLR, double disruptionEnergy, double largestMass, double smallestMass) {
    return 2 * disruptionEnergy*(1-massLR/(largestMass+smallestMass));
}

double impact_velocity(double reducedMass, double specificImpEnergy, double totalMass) {
    return sqrt(2 * specificImpEnergy*totalMass/reducedMass);
}