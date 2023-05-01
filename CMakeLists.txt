cmake_minimum_required(VERSION 3.17)

project(RayTracer)

set(SRCS
    src/main.cpp
    src/Core.cpp
    src/Physics/LightUtils.cpp
    src/Primitives/MeshTriangle/MeshTriangle.cpp
    src/Primitives/Sphere/Sphere.cpp
    src/Parser/ObjParser.cpp
    shared/math/analysis/analysis.cpp
    shared/math/Vectors/VecUtils.cpp
    shared/math/utils.cpp
)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++20")

include_directories(include)

add_executable(raytracer ${SRCS} ${HEADERS})