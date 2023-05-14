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
#include "Parser/Object/ObjParser.hpp"
#include "Parser/Config/CfgParser.hpp"

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
    Parser::ObjParserData::TransformationsOptions opt = {
        .pos = Vec3f(0, 3, 12),
        .color = Vec3f(0.2, 0, 0),
        //.pos = Vec3f(0, 0, -1),
        .scaleFactorX = 0.05,
        .scaleFactorY = 0.05,
        .scaleFactorZ = 0.05,
        .rotateXAxis = 80,
        // .rotateYAxis = -90,
        // .rotateZAxis = -90,
        //.rotateZAxis = 45,
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
    math::translate(o2w, Vec3f(1, -1, 2));
    primitive::Plane *right = new primitive::Plane(o2w, Vec3f(1, 0, 0.5), Vec3f(10, 10, -1));
    right->txtType = primitive::CHECKER;
    //plane->albedo = Vec3f(0.6, 0, 0);
    core.addObject(std::shared_ptr<primitive::Object>(right));
    primitive::Plane *floor = new primitive::Plane(o2w, Vec3f(0, -1, 0), Vec3f(10, 10, -1));
    floor->txtType = primitive::CHECKER;
    core.addObject(std::shared_ptr<primitive::Object>(floor));
    primitive::Plane *left = new primitive::Plane(o2w, Vec3f(-1, 0, 0.9), Vec3f(-10, -10, 1));
    core.addObject(std::shared_ptr<primitive::Object>(left));
    primitive::Plane *ceilling = new primitive::Plane(o2w, Vec3f(0, 1, 0.7), Vec3f(10, 5, -5));
    core.addObject(std::shared_ptr<primitive::Object>(ceilling));
    primitive::Plane *back = new primitive::Plane(o2w, Vec3f(0, 5, 5), Vec3f(0, 0, 0));
    core.addObject(std::shared_ptr<primitive::Object>(back));;
    math::translate(o2w, Vec3f(0, 0.5, -2));
    primitive::Sphere *sph = new primitive::Sphere(o2w, 0.5);
    sph->albedo = Vec3f(0, 0.2, 0);
    core.addObject(std::shared_ptr<primitive::Object>(sph));
}

int main(__attribute__((unused))int argc, __attribute__((unused))char **argv)
{
    // get config
    Parser::CfgParser cfgParser("./src/Parser/Config/scene.cfg");

    Parser::CfgParserData::Camera camera = cfgParser.getCamera();
    Parser::CfgParserData::Primitives prim = cfgParser.getPrimitives();
    Parser::CfgParserData::Lights lights = cfgParser.getLights();

    raytracer::Core core(camera.cameraToWorld, camera.width, camera.height);
    core.camera = camera;

    for (auto &item: prim)
        core.addObject(item);

    for (auto &item: lights)
        core.addLight(item);
    //Setup obj
    Matrix44f objectToWorld;
    Parser::ObjParserData::TransformationsOptions opt = {
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
    //Matrix44f l2w;
    //math::translate(l2w, Vec3f(0, -5, -3));
    //math::rotateAroundOriginY(l2w, math::deg2Rad(45));
    //math::rotateAroundOriginZ(l2w, math::deg2Rad(45));
    // Matrix44f l2w(11.146836, -5.781569, -0.0605886, 0,
    //              -1.902827, -3.543982, -11.895445, 0,
    //              5.459804, 10.568624, -4.02205, 0,
    //              0, 1, 0, 1);
    //core.addLight(std::shared_ptr<physics::Light>(new physics::DistantLight(l2w, 1, 5)));
    //randomScene(core);
    //multipleSphereScene(core);
    // teapotScene(core);

    std::thread render ([&core] () {core.render();});
    // pthread_create(&render, NULL, core.render(), NULL);
    sf::RenderWindow window(sf::VideoMode((int)core.camera.width, (int)core.camera.height), "Raytracer");
    sf::Image image;
    image.create((int)core.camera.width, (int)core.camera.height, sf::Color::Black);
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Event event;
    while (window.isOpen()) {
        fprintf(stderr, "\rStatus: % 7.2f %%", core.getCompletionPercentage());
        convertFrameBuffer((int)core.camera.width, (int)core.camera.height, core.getFrameBuffer(), image);
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                core.stopRender();
                window.close();
            }
            if (event.type == sf::Event::EventType::KeyReleased && event.key.code == sf::Keyboard::Right) {
                core.camera.rotateAroundOriginY(90);
                core.requestRerender();
            }
            if (event.type == sf::Event::EventType::KeyReleased && event.key.code == sf::Keyboard::Up) {
                core.camera.rotateAroundOriginX(90);
                core.requestRerender();
            }
            if (event.type == sf::Event::EventType::KeyReleased && event.key.code == sf::Keyboard::Down) {
                core.camera.rotateAroundOriginX(-90);
                core.requestRerender();
            }
            if (event.type == sf::Event::EventType::KeyReleased && event.key.code == sf::Keyboard::Left) {
                core.camera.rotateAroundOriginY(-90);
                core.requestRerender();
            }
        }
        window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
    }
    fprintf(stderr, "\n");
    core.stopRender();
    render.join(); // block until Done: 100%
/*    pthread_t pthread_id = render.native_handle();
    pthread_cancel(pthread_id);*/
    return 0;
}
