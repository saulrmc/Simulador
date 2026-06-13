cmake_minimum_required(VERSION 4.0)
project(SimuladorGravitacional)

set(CMAKE_CXX_STANDARD 20)


file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "main.cpp")
add_subdirectory(libs/GLFW)
add_library(glad libs/glad/glad.c)
target_include_directories(glad PUBLIC libs)

add_executable(SimuladorGravitacional main.cpp
        src/Physics/PhysicsUtils.h
        src/Physics/CalculationManager.cpp
        src/Physics/CalculationManager.h
        src/Commons/CelestialBodies.h
        src/Commons/Vec3.h
        src/Physics/Physics.cpp
        src/Physics/Physics.h
        src/Physics/Collisions.h
        src/Physics/Collisions.cpp
        src/Physics/LeapfrogKDK.h
        src/Physics/LeapfrogKDK.cpp
        src/GUI/Interface.cpp
        src/GUI/Interface.h
        src/Physics/spatial_structures/Octree/ElementOctree.h
        src/Physics/spatial_structures/Octree/Octree.cpp
        src/Physics/spatial_structures/Octree/Octree.h
        src/Physics/spatial_structures/Octree/NodeOctree.h
        src/Simulation.cpp
        src/Simulation.h
        src/Creation/Creation.cpp
        src/Creation/Creation.h
        src/Physics/spatial_structures/LinearOctree/LinearOctree.cpp
        src/Physics/spatial_structures/LinearOctree/LinearOctree.h
        src/Physics/spatial_structures/LinearOctree/LinearElement.cpp
        src/Physics/spatial_structures/LinearOctree/LinearElement.h
        src/Physics/spatial_structures/LinearOctree/LinearNode.cpp
        src/Physics/spatial_structures/LinearOctree/LinearNode.h
        src/Physics/spatial_structures/SpatialStructure.cpp
        src/Physics/spatial_structures/SpatialStructure.h
)
find_package(OpenGL REQUIRED)
find_package(OpenMP REQUIRED)
target_link_libraries(SimuladorGravitacional glfw glad OpenGL::GL OpenMP::OpenMP_CXX)
target_include_directories(SimuladorGravitacional PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Commons)
