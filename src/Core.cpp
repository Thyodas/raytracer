/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** Core.cpp
*/

#include "Core.hpp"
#include <cmath>
#include <iostream>
#include <fstream>
#include <limits>

namespace raytracer {

    void Core::addObject(const std::shared_ptr<primitive::Object> &obj)
    {
        _scene.addObjectToScene(obj);
    }

    void Core::addLight(const std::shared_ptr<physics::Light> &light)
    {
        _scene.addLightToScene(light);
    }

    void Core::render(void)
    {
        float scale = tan(math::deg2Rad(_fov * 0.5));
        float imageAspectRatio = _width / (float)_height;
        Vec3f orig;
        _cameraToWorld.multVecMatrix(Vec3f(0), orig);
        std::cout << orig << std::endl;
        uint32_t index = 0;
        for (uint32_t j = 0; j < _height; ++j) {
            for (uint32_t i = 0; i < _width; ++i) {
                float x = (2 * (i + 0.5) / (float)_width - 1) * imageAspectRatio * scale;
                float y = (1 - 2 * (j + 0.5) / (float)_height) * scale;
                Vec3f dir;
                _cameraToWorld.multVecMatrix(Vec3f(x, y, -1), dir);
                dir = math::normalize(dir);
                (_framebuffer.get())[index] = _scene.castRay(orig, dir, 0);
                index++;
            }
        }

        std::ofstream ofs;
        ofs.open("./out.ppm");
        ofs << "P6\n" << _width << " " << _height << "\n255\n";
        for (uint32_t i = 0; i < _height * _width; ++i) {
            char r = (char)(255 * math::clamp(0, 1, (_framebuffer.get())[i].x));
            char g = (char)(255 * math::clamp(0, 1, (_framebuffer.get())[i].y));
            char b = (char)(255 * math::clamp(0, 1, (_framebuffer.get())[i].z));
            ofs << r << g << b;
        }

        ofs.close();
    }
}