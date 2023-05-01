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

    objects.push_back(std::unique_ptr<primitive::Sphere>(sph1));
    objects.push_back(std::unique_ptr<primitive::Sphere>(sph2));

    Vec3f verts[4] = {{-5,-3,-6}, {5,-3,-6}, {5,-3,-16}, {-5,-3,-16}};
    uint32_t vertIndex[6] = {0, 1, 3, 1, 2, 3};
    Vec2f st[4] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    primitive::MeshTriangle *mesh = new primitive::MeshTriangle(verts, vertIndex, 2, st);
    mesh->refractionCoefficient = 1.5;
    mesh->materialType = primitive::DIFFUSE_AND_GLOSSY;

    objects.push_back(std::unique_ptr<primitive::MeshTriangle>(mesh));

    lights.push_back(std::unique_ptr<physics::Light>(new physics::Light(Vec3f(-20, 70, 20), 0.5)));
    lights.push_back(std::unique_ptr<physics::Light>(new physics::Light(Vec3f(30, 50, -12), 1)));

    raytracer::Options options;
    options.width = 640;
    options.height = 480;
    options.fov = 30;
    options.backgroundColor = Vec3f(0.0, 0.0, 1);
    options.maxDepth = 5;
    options.bias = 0.00001;

    Parser::parseObj("./src/Parser/rose_cube.obj", true);
    raytracer::render(options, objects, lights);
}
