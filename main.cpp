#include <iostream>
#include "src/Physics/CalculationManager.h"
#include <string>
#include "src/Creation/CelestialBody.h"

//full hardcodeo, aun sin respetar al 100% la arquitectura xd
int main() {
    int opcion;
    CalculationManager gestor_calculos;

    while (true) {
        gestor_calculos.simulation_begin();
        std::cout << "VERSION DE CONSOLA DEL SIMULADOR"<<std::endl;
        std::cout << "OPCIONES: "<<std::endl<<"1) Agregar cuerpo celeste:"<<std::endl;
        std::cout << "2) Eliminar cuerpo celeste:"<<std::endl<< "3)Ver simulacion"<<std::endl<<"4) Salir"<<std::endl;
        std::cin >> opcion;
        if (opcion == 1) {
            double masa;
            std::cout << "Ingrese la masa (en masas terrestres)"<<std::endl;
            std::cin >> masa;
            if (masa>0) {
                std::string nombre{};
                double posX, posY, posZ;
                double radio;
                std::cout << "Ingrese el nombre del cuerpo" <<  std::endl;
                std::cin >> nombre;
                std::cout << "Ingrese la posicion (x, y, z):"<<std::endl;
                std::cin >> posX >> posY >> posZ;
                Vec3 posicion(posX, posY, posZ);
                Vec3 velocidad(1, 1, 1);
                std::cout << "Ingrese el radio (en miles de km):"<<std::endl;
                std::cin >> radio;
                if (!nombre.empty()) {
                    CelestialBody cuerpo_celeste(nombre, posicion, velocidad, masa, radio);
                }
                else {
                    std::cout << "sin nombre"<<std::endl;
                    CelestialBody cuerpo_celeste("1", posicion, velocidad, masa, radio);
                }

            }
            else {
                std::cout << "masa incorrecta"<<std::endl;
            }
        }
        else if (opcion == 2) {
            //por ahora no está esta opcion xd
        }
        else if (opcion == 3) {
            gestor_calculos.show_simulation();
        }
        else {
            gestor_calculos.simulation_ends();
            break;
        }
    }
    return 0;
}
