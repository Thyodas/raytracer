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

        uint64_t index = 0;
        float i, j;
        while ((index = _getNewWriteIndex()) < _framebufferSize) {
            i = (float)(index % _width);
            j = (float)(index / _width);
            Vec3f dir;
            camera.getDir(i, j, dir);
            (_framebuffer.get())[index] = _scene.castRay(camera.orig, dir, 0);
            if (checkRerender())
                return 1;
            if (checkStopRender())
                return 2;
        }
        if (index != _framebufferSize)
            return 0;
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
        std::vector <std::thread> threads;
        for (unsigned int i = 0; i < _nbThreads - 1; ++i) {
            std::thread worker([this]() {
                int returnValue;
                while (1) {
                    if (_getWriteIndex() == 0) {
                        for (uint32_t i = 0 ; i < _framebufferSize ; ++i)
                            _framebuffer.get()[i] = {0};
                    };
                    returnValue = _executeRender();
                    if (returnValue == 1)
                        continue;
                    if (returnValue == 2)
                        break;
                    waitRerender();
                    if (checkStopRender())
                        break;
                };
            });
            threads.push_back(std::move(worker));
        }
        for(auto &t: threads){
            t.join();
        }
    }

    uint64_t Core::_getNewWriteIndex(void)
    {
        uint64_t newWriteIndex;
        _writeIndexMutex.lock();
        newWriteIndex = _writeIndex++;
        _writeIndexMutex.unlock();
        return newWriteIndex;
    }

    void Core::_resetWriteIndex(void)
    {
        _writeIndexMutex.lock();
        _writeIndex = 0;
        _writeIndexMutex.unlock();
    }

    uint64_t Core::_getWriteIndex(void)
    {
        _writeIndexMutex.lock();
        _writeIndex;
        _writeIndexMutex.unlock();
        return _writeIndex;
    }

    float Core::getCompletionPercentage(void)
    {
        return (float)_getWriteIndex() / (float)_framebufferSize * 100;
    }
}
