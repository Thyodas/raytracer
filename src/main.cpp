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
    //Setup Core
    Matrix44f cameraToWorld = Matrix44f(1, 0, 0, 0,
                                        0, 1, 0, 0,
                                        0, 0, 1, 0,
                                        0, 3, 10, 1);
    raytracer::Core core(cameraToWorld);
    core.setFov(90);

    //Setup obj
    Matrix44f objectToWorld;
    Parser::ObjParserData::transformationsOptions opt = {
            .pos = Vec3f(0, 1, -1),
            // .scaleFactorX = 2.5,
            // .scaleFactorY = 2.5,
            // .scaleFactorZ = 2.5,
            //.rotateXAxis = 85,
            // .rotateYAxis = -90,
            // .rotateZAxis = -90,
            // .rotateZAxis = 45,
    };
    Parser::parseObj(core, opt, "./plane.obj", true);

    //Setup lights
    Matrix44f l2w(11.146836, -5.781569, -0.0605886, 0,
                 -1.902827, -3.543982, -11.895445, 0,
                 5.459804, 10.568624, -4.02205, 0,
                 0, 1, 0, 1);
    core.addLight(std::shared_ptr<physics::Light>(new physics::DistantLight(l2w, 1, 1)));
    float w[5] = {0.04, 0.08, 0.1, 0.15, 0.2};
    Vec3f c[5] = {Vec3f(0.2, 0, 0), Vec3f(0, 0.2, 0),Vec3f(0,0,0.2), Vec3f(0.2,0.2,0), Vec3f(0.2, 0.2,0.2)};
    for (int i = -4, n = 2, k = 0; i <= 4; i+= 2, n *= 5, k++) {
        Matrix44f xformSphere;
        xformSphere[3][0] = i;
        xformSphere[3][1] = 2;
        xformSphere[3][2] = 5;
         primitive::Sphere *sph = new primitive::Sphere(xformSphere, 0.9);
        sph->specularExponent = n;
        sph->ks = w[k];
        sph->albedo = c[k];
        core.addObject(std::shared_ptr<primitive::Object>(sph));
    }


    core.render();
    return 0;
}
