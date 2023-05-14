/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** Core.hpp
*/

#pragma once

#include "Object.hpp"
#include "Light.hpp"
#include "Primitives/Sphere/Sphere.hpp"
#include "Primitives/MeshTriangle/MeshTriangle.hpp"
#include "../shared/math/utils.hpp"
#include "../shared/math/Matrix/Matrix44.hpp"
#include "Physics/LightUtils.hpp"
#include "Scene/Scene.hpp"
#include "Camera/Camera.hpp"

#include <memory>
#include <semaphore>
#include <thread>

namespace raytracer {
    class Core {
        public:
            Core(Matrix44f &c2w,
                uint32_t width = 600,
                uint32_t height = 400,
                float fov = 90,
                uint8_t maxDepth = 2,
                float bias = 0.001,
                Vec3f backgroundColor = Vec3f(0.5, 0.5, 0.5)) :
                camera(c2w, width, height, fov),
                _scene(maxDepth, bias, backgroundColor),
                _rerenderSemaphore(0),
                _writeIndex(0),
                _framebufferSize(width * height),
                _width(width),
                _height(height),
                _nbThreads(std::thread::hardware_concurrency()),
                _stopRender(false)
            {
                _framebuffer = std::unique_ptr<Vec3f>(new Vec3f[_framebufferSize]);
            };

            void setWidth(uint32_t width) { camera.width = width;}
            uint32_t getWidth(void) const { return camera.width;}
            void setHeight(uint32_t height) { camera.height = height;}
            uint32_t getHeight(void) const { return camera.height;}
            void setFov(float fov) { camera.fov = fov;}
            float getFov(void) const { return camera.fov;}
            void setMaxDepth(uint8_t maxDepth) { _scene.setMaxDepth(maxDepth);}
            uint8_t getMaxDepth(void) const { return _scene.getMaxDepth();}
            void setBias(float bias) { _scene.setBias(bias);}
            float getBias(void) const { return _scene.getBias();}
            void setBackGroundColor(Vec3f &backgroundColor) { _scene.setBackGroundColor(backgroundColor);}
            Vec3f getBackGroundColor(void) const { return _scene.getBackGroundColor();}
            std::shared_ptr<Vec3f> getFrameBuffer(void) const { return _framebuffer;}

            void requestRerender(void) {
                _resetWriteIndex();
                _rerenderSemaphore.release(_nbThreads - 1);
            }

            bool waitRerender(void) {
                _rerenderSemaphore.acquire();
                return true;
            }

            bool checkRerender(void) {
                return _rerenderSemaphore.try_acquire();
            }

            void stopRender(void) {
                _stopRender = true;
                requestRerender();
            }
            bool checkStopRender(void) {
                return _stopRender;
            }

            void render(void);
            void addObject(const std::shared_ptr<primitive::Object> &obj);
            void addLight(const std::shared_ptr<physics::Light> &light);
            float getCompletionPercentage(void);

        public:
            Camera camera;
        private:
            Scene _scene;
            std::shared_ptr<Vec3f> _framebuffer;
            std::binary_semaphore _rerenderSemaphore;
            std::mutex _writeIndexMutex;
            uint64_t _writeIndex;
            uint64_t _framebufferSize;
            uint64_t _width;
            uint64_t _height;
            long _nbThreads;
            bool _stopRender;

            int _executeRender(void);
            uint64_t _getWriteIndex(void);
            uint64_t _getNewWriteIndex(void);
            void _resetWriteIndex(void);
    };
}