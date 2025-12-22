cmake_minimum_required(VERSION 4.0)
project(SimuladorGravitacional)

set(CMAKE_CXX_STANDARD 20)

set(SFML_DIR "${CMAKE_SOURCE_DIR}/src/Visualizacion/libs/SFML")

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "main.cpp")
add_executable(SimuladorGravitacional main.cpp
        src/MotorFísico/MotorFisicoUtils.h
        src/MotorFísico/GestorCalculos.cpp
        src/MotorFísico/GestorCalculos.h
        src/MotorFísico/CuerpoCeleste.cpp
        src/MotorFísico/CuerpoCeleste.h)
target_include_directories(SimuladorGravitacional PRIVATE "${SFML_DIR}/include")

target_link_libraries(SimuladorGravitacional PRIVATE
        "${SFML_DIR}/lib/libsfml-graphics.a"
        "${SFML_DIR}/lib/libsfml-window.a"
        "${SFML_DIR}/lib/libsfml-system.a"
        -static-libgcc
        -static-libstdc++
        -static
        -lpthread
)
if(WIN32)
    add_custom_command(TARGET SimuladorGravitacional POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/src/Visualizacion/libs/SFML/bin"
            $<TARGET_FILE_DIR:SimuladorGravitacional>
            COMMENT "Copiando librerías dinámicas al directorio de ejecución..."
    )
endif()