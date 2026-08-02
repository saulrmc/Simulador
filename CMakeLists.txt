cmake_minimum_required(VERSION 4.0)
project(SimuladorGravitacional)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "main.cpp")
add_subdirectory(libs/GLFW)
#set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
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
        src/Physics/spatial_structures/Octree/Octree.h
        src/Physics/spatial_structures/Octree/NodeOctree.h
        src/Simulation.cpp
        src/Simulation.h
        src/Creation/Creation.cpp
        src/Creation/Creation.h
        src/Physics/spatial_structures/LinearOctree/LinearOctree.h
        src/Physics/spatial_structures/LinearOctree/LinearElement.h
        src/Physics/spatial_structures/SpatialStructure.h
        src/Physics/spatial_structures/geometry.h
        src/Physics/spatial_structures/geometry.cpp
        src/Physics/spatial_structures/Octree/Octree.cpp
        src/Physics/spatial_structures/Octree/NodeOctree.cpp
        src/Physics/spatial_structures/LinearOctree/LinearOctree.cpp
)
# LinearElement.h usa intrinsics BMI2 (_pdep_u64), así que solo ese archivo
# necesita el flag; el resto del proyecto compila sin exigir BMI2.
set_source_files_properties(src/Physics/spatial_structures/LinearOctree/LinearOctree.cpp
    PROPERTIES COMPILE_OPTIONS "-mbmi2")
find_package(OpenGL REQUIRED)
find_package(OpenMP REQUIRED)
target_link_libraries(SimuladorGravitacional glfw glad OpenGL::GL OpenMP::OpenMP_CXX)
target_include_directories(SimuladorGravitacional PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Commons)
# La estructura espacial activa se elige en src/Physics/spatial_structures/config.h
