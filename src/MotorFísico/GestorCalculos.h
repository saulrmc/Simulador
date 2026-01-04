//
// Created by Saúl on 21/12/2025.
//

#ifndef SIMULADORGRAVITACIONAL_GESTORCALCULOS_H
#define SIMULADORGRAVITACIONAL_GESTORCALCULOS_H

#include "MotorFisicoUtils.h"
class GestorCalculos {
    //El producto de software deberá recalcular y mostrar instantáneamente
    //las interacciones y trayectorias afectadas por cualquier modificacion
    //(incluyendo eliminación) de los cuerpos celestes dentro de la simulación.
    //Esto debido a que el cambio en las atracciones gravitatorias
    //entre cuerpos generan nuevas interacciones.
    public:
    GestorCalculos();
    virtual ~GestorCalculos();
    //Pasos:
    //tener un vector de cuerpos
    //detectar un cambio para recalcular
    //
    private:

};


#endif //SIMULADORGRAVITACIONAL_GESTORCALCULOS_H