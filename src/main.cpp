/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** main.cpp
*/

#include "Object.hpp"
#include "Light.hpp"
#include "Primitives/Sphere/Sphere.hpp"
#include "Primitives/MeshTriangle/MeshTriangle.hpp"
#include "Core.hpp"
#include "../shared/math/Vectors/Vec3.hpp"
#include "../shared/math/Vectors/Vec2.hpp"
#include "Parser.hpp"
#include "Parser/ObjParser.hpp"

#include <vector>
#include <memory>

int main(int argc, char **argv)
{
    raytracer::Core core;
    core.setFov(30);
    std::vector<std::unique_ptr<primitive::Object>> objects;
    std::vector<std::unique_ptr<physics::Light>> lights;

    primitive::Sphere *sph1 = new primitive::Sphere(Vec3f(-1, 0, -12), 2);
    sph1->materialType = primitive::DIFFUSE_AND_GLOSSY;
    sph1->diffuseColor = Vec3f(0.6, 0.0, 0.0);
    primitive::Sphere *sph2 = new primitive::Sphere(Vec3f(0.9, -0.5, -8), 1.5);
    sph2->refractionCoefficient = 1.5;
    sph2->kd = 0.5;
    sph2->ks = 0.5;
    //sph2->diffuseColor = Vec3f(0.6, 0, 0);
    sph2->materialType = primitive::REFLECTION_AND_REFRACTION;

    std::shared_ptr<primitive::Sphere> s1 = std::make_shared<primitive::Sphere>(*sph1);
    core.addObject(s1);
    core.addObject(std::shared_ptr<primitive::Sphere>(sph2));

    std::vector<Vec3f> verts = {{-5,-3,-6}, {5,-3,-6}, {5,-3,-16}, {-5,-3,-16}};
    std::vector<uint32_t> vertIndex = {0, 1, 3, 1, 2, 3};
    std::vector<Vec2f> st = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    primitive::MeshTriangle *mesh = new primitive::MeshTriangle(verts, vertIndex, 2, st);
    mesh->refractionCoefficient = 1.5;
    mesh->materialType = primitive::DIFFUSE_AND_GLOSSY;

    core.addObject(std::shared_ptr<primitive::MeshTriangle>(mesh));

    core.addLight(std::shared_ptr<physics::Light>(new physics::Light(Vec3f(-20, 70, 20), 0.5)));
    core.addLight(std::shared_ptr<physics::Light>(new physics::Light(Vec3f(30, 50, -12), 1)));

    //Parser::parseObj("./src/Parser/rose_cube.obj", false);
    core.render();
}
