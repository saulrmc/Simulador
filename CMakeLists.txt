cmake_minimum_required(VERSION 4.0)
project(SimuladorGravitacional)

set(CMAKE_CXX_STANDARD 20)


file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "main.cpp")
add_subdirectory(libs/GLFW)
add_library(glad libs/glad/glad.c
        src/GUI/Interface.cpp
        src/GUI/Interface.h
        src/Physics/Octree/ElementOctree.cpp
        src/Physics/Octree/ElementOctree.h
        src/Physics/Octree/Octree.cpp
        src/Physics/Octree/Octree.h
        src/Physics/Octree/NodeOctree.cpp
        src/Physics/Octree/NodeOctree.h)
target_include_directories(glad PUBLIC libs)

add_executable(SimuladorGravitacional main.cpp
        src/Physics/PhysicsUtils.h
        src/Physics/CalculationManager.cpp
        src/Physics/CalculationManager.h
        src/Creation/CelestialBody.cpp
        src/Creation/CelestialBody.h
        src/Visualization/Simulation.cpp
        src/Creation/Vec3.cpp
        src/Creation/Vec3.h
        src/Physics/Physics.cpp
        src/Physics/Physics.h
)

target_link_libraries(SimuladorGravitacional glfw glad opengl32)
