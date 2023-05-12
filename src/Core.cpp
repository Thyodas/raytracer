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
#include <chrono>

namespace raytracer {

    void Core::addObject(const std::shared_ptr<primitive::Object> &obj)
    {
        _scene.addObjectToScene(obj);
    }

    void Core::addLight(const std::shared_ptr<physics::Light> &light)
    {
        _scene.addLightToScene(light);
    }

    int Core::_executeRender(void)
    {
        uint32_t index = 0;
        auto timeStart = std::chrono::high_resolution_clock::now();
        std::cout << camera.orig << std::endl;
        for (uint32_t j = 0; j < camera.height; ++j) {
            for (uint32_t i = 0; i < camera.width; ++i, ++index) {
                Vec3f dir;
                camera.getDir(i, j, dir);
                (_framebuffer.get())[index] = _scene.castRay(camera.orig, dir, 0);
                if (checkRerender())
                    return 1;
                if (checkStopRender())
                    return 2;
            }
            fprintf(stderr, "\r%3d%c", uint32_t(j / (float)camera.height * 100), '%');
        }
        auto timeEnd = std::chrono::high_resolution_clock::now();
        auto passedTime = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();
        fprintf(stderr, "\rDone: %.2f (sec)\n", passedTime / 1000);
        std::ofstream ofs;
        ofs.open("./out.ppm");
        ofs << "P6\n" << camera.width << " " << camera.height << "\n255\n";
        for (uint32_t i = 0; i < camera.height * camera.width; ++i) {
            char r = (char)(255 * math::clamp(0, 1, (_framebuffer.get())[i].x));
            char g = (char)(255 * math::clamp(0, 1, (_framebuffer.get())[i].y));
            char b = (char)(255 * math::clamp(0, 1, (_framebuffer.get())[i].z));
            ofs << r << g << b;
        }
        ofs.close();
        return 0;
    }

    void Core::render(void)
    {
        int returnValue = 0;
        while (1) {
            returnValue = _executeRender();
            if (returnValue == 1) {
                for (uint32_t i = 0; i < camera.height * camera.width; ++i)
                    _framebuffer.get()[i] = {0};
            }
            if (returnValue == 2) {
                // pthread_cancel(pthread_self());
                break;
            }
        };
    }
}
