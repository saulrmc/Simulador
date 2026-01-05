cmake_minimum_required(VERSION 4.0)
project(SimuladorGravitacional)

set(CMAKE_CXX_STANDARD 20)


file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "main.cpp")
add_subdirectory(libs/GLFW)
add_library(glad libs/glad/glad.c
        src/GUI/Interfaz.cpp
        src/GUI/Interfaz.h)
target_include_directories(glad PUBLIC libs)

add_executable(SimuladorGravitacional main.cpp
        src/MotorFísico/MotorFisicoUtils.h
        src/MotorFísico/GestorCalculos.cpp
        src/MotorFísico/GestorCalculos.h
        src/GestionCreacion/CuerpoCeleste.cpp
        src/GestionCreacion/CuerpoCeleste.h
        src/Visualizacion/Simulacion.cpp
)

target_link_libraries(SimuladorGravitacional glfw glad opengl32)
