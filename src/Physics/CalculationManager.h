//
// Created by Saúl on 21/12/2025.
//

#ifndef SIMULADORGRAVITACIONAL_GESTORCALCULOS_H
#define SIMULADORGRAVITACIONAL_GESTORCALCULOS_H
#include "../Creation/CelestialBody.h"
#include "PhysicsUtils.h"
class CalculationManager {
    public:
    CalculationManager();
    virtual ~CalculationManager();
    void simulacion_inicia();
    void mostrar_simulacion();
    void simulacion_termina();
    void insertar_cuerpo();
    void eliminar_cuerpo();

    private:
    std::vector<CelestialBody> cuerpos{};
};


#endif //SIMULADORGRAVITACIONAL_GESTORCALCULOS_H