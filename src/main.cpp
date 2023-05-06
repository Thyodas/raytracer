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
#include "../shared/math/Matrix/Matrix44.hpp"
#include "Parser.hpp"
#include "Parser/ObjParser.hpp"

#include <vector>
#include <memory>

int main(int argc, char **argv)
{
    Matrix44f cameraToWorld = Matrix44f(1, 0, 0, 0,
                                        0, 1, 0, 0,
                                        0, 0, 1, 0,
                                        0, 1, 12, 1);
    raytracer::Core core(cameraToWorld);
    core.setFov(36.87);
    Matrix44f objectToWorld;

    // std::vector<Vec3f> verts = {{-30,-15,-6}, {30,-15,-6}, {30,-15,-86}, {-30,-15,-86}};
    // std::vector<uint32_t> vertIndex = {0, 1, 3, 1, 2, 3};
    // std::vector<int> faceIndex = {3, 3};
    // std::vector<Vec2f> st = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    // primitive::MeshTriangle *mesh = new primitive::MeshTriangle(cameraToWorld, faceIndex, vertIndex, verts);
    // mesh->refractionCoefficient = 1.5;
    // mesh->materialType = primitive::DIFFUSE_AND_GLOSSY;

    //core.addObject(std::shared_ptr<primitive::MeshTriangle>(mesh));

    objectToWorld[3][0] = 1;
    objectToWorld[3][1] = 1.5;
    objectToWorld[3][2] = 2;
    primitive::Sphere *sph1 = new primitive::Sphere(objectToWorld, 1);
    sph1->materialType = primitive::REFLECTION_AND_REFRACTION;
    sph1->diffuseColor = Vec3f(0.6, 0.0, 0.0);
    sph1->kd = 0.1;
    sph1->ks = 0.5;
    sph1->refractionCoefficient = 1.5;

    Matrix44f xformSphere;
    xformSphere[3][0] = 0.5;
    xformSphere[3][1] = 1;
    xformSphere[3][2] = 5;
    primitive::Sphere *sph2 = new primitive::Sphere(xformSphere, 0.5);
    sph2->specularExponent = 50;
    sph2->ks = 2;
    // sph2->refractionCoefficient = 1.5;
    sph2->kd = 0.8;
    // sph2->ks = 0.5;
    sph2->diffuseColor = Vec3f(1, 0, 0);
    sph2->materialType = primitive::DIFFUSE_AND_GLOSSY;

    std::shared_ptr<primitive::Sphere> s1 = std::make_shared<primitive::Sphere>(*sph1);
    //core.addObject(s1);
    core.addObject(std::shared_ptr<primitive::Sphere>(sph2));


    Parser::ObjParserData::transformationsOptions opt = {
        .pos = Vec3f(0, 0, -1),
        // .scaleFactorX = 2.5,
        // .scaleFactorY = 2.5,
        // .scaleFactorZ = 2.5,
        //.rotateXAxis = 85,
        // .rotateYAxis = -90,
        // .rotateZAxis = -90,
        // .rotateZAxis = 45,
    };

    Matrix44f l2w(11.146836, -5.781569, -0.0605886, 0,
                -1.902827, -3.543982, -11.895445, 0,
                5.459804, 10.568624, -4.02205, 0,
                0, 0, 0, 1);

    Matrix44f l2wAboveScene(0, 0, 1, 0,
                            0, 1, 0, 0,
                            -1, 0, 0, 0,
                            -1, -2, 15, 1);

    // Matrix44f l2w(1, 0, 0, 0,
    //               0, 1, 0, 0,
    //               0, 0, 1 ,0,
    //               0, 0, 0, 1);

    core.addLight(std::shared_ptr<physics::Light>(new physics::DistantLight(l2w, 1, 1)));
    //core.addLight(std::shared_ptr<physics::Light>(new physics::DistantLight(l2w, 0.3, 10)));

    Parser::parseObj(core, opt, "./plane.obj", false);

    opt.pos = Vec3f(-2, 2, -5);
    opt.rotateXAxis = 90;
    opt.scaleFactorX = 0.1;
    opt.scaleFactorY = 0.1;
    opt.scaleFactorZ = 0.1;
    //Parser::parseObj(core, opt, "./teapot.obj", false);
    core.render();
}
