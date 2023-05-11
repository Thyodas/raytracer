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
#include "Primitives/Plane/Plane.hpp"
#include "Core.hpp"
#include "../shared/math/Vectors/Vec3.hpp"
#include "../shared/math/Vectors/Vec2.hpp"
#include "../shared/math/Matrix/MatrixTransformation.hpp"
#include "../shared/math/Matrix/Matrix44.hpp"
#include "Parser.hpp"
#include "Parser/ObjParser.hpp"

#include <thread>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

inline float randomFloat()
{
    return rand() / (RAND_MAX + 1.0);
}

inline double randomFloat(double min, double max)
{
    return min + (max - min) * randomFloat();
}

void randomScene(raytracer::Core &core)
{
    srand(time(NULL));
    for (int i = -2; i < 2; ++i) {
        for (int j = -2; j < 2; ++j) {
            float randomMat = randomFloat();
            Vec3f center(i + randomFloat(-10, 10), 3, j +  randomFloat(-10, 10));
            Matrix44f xFormSphere;
            xFormSphere[3][0] = center.x;
            xFormSphere[3][1] = center.y;
            xFormSphere[3][2] = center.z;
            if (randomMat < 0.8) {
                Vec3f randomColor(randomFloat(), randomFloat(), randomFloat());
                primitive::Sphere *sph = new primitive::Sphere(xFormSphere, randomFloat(0.5, 2));
                sph->albedo = randomColor;
                sph->kd = randomFloat(0.05, 0.5);
                sph->ks = randomFloat(0.05, 0.5);
                sph->materialType = primitive::DIFFUSE_AND_GLOSSY;
                sph->specularExponent = randomFloat(2, 200);
                core.addObject(std::shared_ptr<primitive::Object>(sph));
            } else {
                primitive::Sphere *sph = new primitive::Sphere(xFormSphere, randomFloat(0.5, 2));
                sph->refractionCoefficient = randomFloat(0.5, 2);
                sph->materialType = primitive::REFLECTION_AND_REFRACTION;
                core.addObject(std::shared_ptr<primitive::Object>(sph));
            }
        }
    }
}

void multipleSphereScene(raytracer::Core &core)
{
    core.camera.translate(Vec3f(0, 2, 8));
    float w[5] = {0.04, 0.08, 0.1, 0.2, 0.4};
    //float z[5] = {5, 8, 10, 7, 6};
    float radius = 0.9;
    Vec3f c[5] = {Vec3f(0.2, 0, 0), Vec3f(0, 0.2, 0),Vec3f(0,0,0.2), Vec3f(0.2,0.2,0), Vec3f(0.2, 0.2,0.2)};
    for (int i = -4, n = 2, k = 0; i <= 4; i+= 2, n *= 2, k++) {
        Matrix44f xformSphere;
        xformSphere[3][0] = i;
        xformSphere[3][1] = 2;
        xformSphere[3][2] = 5;
        // if (i == 2) {
        //     xformSphere[3][0] = 0;
        //     xformSphere[3][2] = 7;
        //     xformSphere[3][1] = 3.8;
        //     radius = 0.5;
        // }
        primitive::Sphere *sph = new primitive::Sphere(xformSphere, radius);
        if (i == -4) {
            //sph->materialType = primitive::REFLECTION_AND_REFRACTION;
            sph->txtType = primitive::CHECKER;
        }
        sph->specularExponent = n;
        sph->ks = w[k];
        sph->albedo = c[k];
        core.addObject(std::shared_ptr<primitive::Object>(sph));
        radius = 0.9;
    }
}

void oneDiffuseSphereOneRefractionSphere(raytracer::Core &core)
{
    Matrix44f transformSph1;
    math::translate(transformSph1, Vec3f(-1, 0, -12));
    primitive::Sphere *sph1 = new primitive::Sphere(transformSph1, 2);
    sph1->materialType = primitive::DIFFUSE_AND_GLOSSY;
    sph1->albedo = Vec3f(0.6, 0.0, 0.0);
    Matrix44f transformSph2;
    math::translate(transformSph2, Vec3f(0.9, -0.5, -8));
    primitive::Sphere *sph2 = new primitive::Sphere(transformSph2, 1.5);
    sph2->refractionCoefficient = 1.5;
    sph2->kd = 0.5;
    sph2->ks = 0.5;
    sph2->materialType = primitive::REFLECTION_AND_REFRACTION;

    core.addObject(std::shared_ptr<primitive::Object>(sph1));
    core.addObject(std::shared_ptr<primitive::Object>(sph2));
}

void teapotScene(raytracer::Core &core)
{
    core.camera.translate(Vec3f(0, 5, 8));
    Parser::ObjParserData::transformationsOptions opt = {
        .pos = Vec3f(0, 3, 12),
        //.pos = Vec3f(0, 0, -1),
        .scaleFactorX = 0.05,
        .scaleFactorY = 0.05,
        .scaleFactorZ = 0.05,
        .rotateXAxis = 80,
        // .rotateYAxis = -90,
        // .rotateZAxis = -90,
        //.rotateZAxis = 45,
        .color = Vec3f(0.2, 0, 0),
    };
    Parser::parseObj(core, opt, "./teapot.obj", false);
    Matrix44f transformSph1;
    math::translate(transformSph1, Vec3f(2, 2, 5));
    primitive::Sphere *sph1 = new primitive::Sphere(transformSph1, 2);
    sph1->materialType = primitive::DIFFUSE_AND_GLOSSY;
    sph1->albedo = Vec3f(0.2, 0.0, 0.0);
    core.addObject(std::shared_ptr<primitive::Object>(sph1));
}

void convertFrameBuffer(int width, int height, std::shared_ptr<Vec3f> frameBuffer, sf::Image &image)
{
    int i = 0;
    for (int line = 0; line < height; ++line) {
        for (int col = 0; col < width; ++col) {
            sf::Uint8 r = (sf::Uint8)(255 * math::clamp(0, 1, (frameBuffer.get())[i].x));
            sf::Uint8 g = (sf::Uint8)(255 * math::clamp(0, 1, (frameBuffer.get())[i].y));
            sf::Uint8 b = (sf::Uint8)(255 * math::clamp(0, 1, (frameBuffer.get())[i].z));
            image.setPixel(col, line, {r, g, b, 255});
            ++i;
        }
    }
}

void plane_scene(raytracer::Core &core)
{
    Matrix44f o2w;
    primitive::Plane *plane = new primitive::Plane(o2w, Vec3f(0, 0, -1), Vec3f(0, 0, 1), Vec3f(0, -1, 0), Vec3f(0, 0, -1));
    plane->albedo = Vec3f(0.6, 0, 0);
    core.addObject(std::shared_ptr<primitive::Object>(plane));
}

int main(__attribute__((unused))int argc, __attribute__((unused))char **argv)
{
    //Setup Core
    // Matrix44f cameraToWorld = Matrix44f(1, 0, 0, 0,
    //                                     0, 1, 0, 0,
    //                                     0, 0, 1, 0,
    //                                     0, 3, 12, 1);
    Matrix44f cameraToWorld;
    // math::rotateAroundOriginY(cameraToWorld, math::deg2Rad(45));
    // math::translate(cameraToWorld, Vec3f(0, 5, 8));
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
    //Parser::parseObj(core, opt, "./plane.obj", true);

    //Setup lights
    Matrix44f l2w;
    //math::translate(l2w, Vec3f(0, -5, -3));
    //math::rotateAroundOriginY(l2w, math::deg2Rad(45));
    //math::rotateAroundOriginZ(l2w, math::deg2Rad(45));
    // Matrix44f l2w(11.146836, -5.781569, -0.0605886, 0,
    //              -1.902827, -3.543982, -11.895445, 0,
    //              5.459804, 10.568624, -4.02205, 0,
    //              0, 1, 0, 1);
    core.addLight(std::shared_ptr<physics::Light>(new physics::PointLight(l2w, Vec3f(0.5, 0, 0), 1)));
    //randomScene(core);
    multipleSphereScene(core);
    // teapotScene(core);

    std::thread render ([&core] () {core.render();});

    sf::RenderWindow window(sf::VideoMode((int)core.camera.width, (int)core.camera.height), "Raytracer");
    sf::Image image;
    image.create(800, 600, sf::Color::Black);
    sf::Texture texture;
    sf::Sprite sprite;
    while (window.isOpen()) {
        sf::Event event;
        convertFrameBuffer((int)core.camera.width, (int)core.camera.height, core.getFrameBuffer(), image);
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                std::cout << "UP" << std::endl;
            if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                std::cout << "RIGHT" << std::endl;
            if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                std::cout << "DOWN" << std::endl;
            if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                std::cout << "LEFT" << std::endl;
        }
        window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
    }
    render.join();
    // core.render();
    return 0;
}
