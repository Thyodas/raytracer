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

#include <memory>

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
                 _scene(maxDepth, bias, backgroundColor),
                 _width(width),
                 _height(height),
                 _fov(fov),
                _cameraToWorld(c2w)
            {
                _framebuffer = std::unique_ptr<Vec3f>(new Vec3f[width * height]);
            };

            void setWidth(uint32_t width)
            {
                _width = width;
            }
            uint32_t getWidth(void) const
            {
                return _width;
            }
            void setHeight(uint32_t height)
            {
                _height = height;
            }
            uint32_t getHeight(void) const
            {
                return _height;
            }
            void setFov(float fov)
            {
                _fov = fov;
            }
            float getFov(void) const
            {
                return _fov;
            }
            void setMaxDepth(uint8_t maxDepth)
            {
                _scene.setMaxDepth(maxDepth);
            }
            uint8_t getMaxDepth(void) const
            {
                return _scene.getMaxDepth();
            }
            void setBias(float bias)
            {
                _scene.setBias(bias);
            }
            float getBias(void) const
            {
                return _scene.getBias();
            }
            void setBackGroundColor(Vec3f &backgroundColor)
            {
                _scene.setBackGroundColor(backgroundColor);
            }
            Vec3f getBackGroundColor(void) const
            {
                return _scene.getBackGroundColor();
            }

            void render(void);
            void addObject(const std::shared_ptr<primitive::Object> &obj);
            void addLight(const std::shared_ptr<physics::Light> &light);
        private:
            Scene _scene;
            Matrix44f _cameraToWorld;
            std::unique_ptr<Vec3f> _framebuffer;

            uint32_t _width;
            uint32_t _height;
            float _fov;
    };
}