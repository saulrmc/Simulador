cmake_minimum_required(VERSION 4.0)
project(SimuladorGravitacional)

set(CMAKE_CXX_STANDARD 20)

add_executable(SimuladorGravitacional main.cpp
        src/CuerpoCeleste.cpp
        src/CuerpoCeleste.h)
