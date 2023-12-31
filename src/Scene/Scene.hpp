/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** Scene.hpp
*/

#pragma once

#include "../../shared/math/Vectors/Vec3.hpp"
#include "../../shared/math/Vectors/VecUtils.hpp"
#include "../../shared/math/utils.hpp"
#include "../../shared/math/MonteCarlo/MonteCarlo.hpp"
#include "../Physics/LightUtils.hpp"
#include "Object.hpp"
#include "Light.hpp"

#include <iostream>
#include <vector>
#include <memory>
#include <limits>

namespace raytracer {
    class Scene : public primitive::Object {
        public:
            ~Scene() = default;
            Scene(uint8_t maxDepth, float bias, Vec3f backGroundColor) : _maxDepth(maxDepth), _bias(bias), _backgroundColor(backGroundColor), Object(Matrix44f()) {};

            Vec3f castRay(const Vec3f &orig, const Vec3f &dir, uint32_t depth);

            void addObjectToScene(const std::shared_ptr<primitive::Object> &obj);
            void addLightToScene(const std::shared_ptr<physics::Light> &light);

            void setMaxDepth(uint8_t maxDepth)
            {
                _maxDepth = maxDepth;
            }
            uint8_t getMaxDepth(void) const
            {
                return _maxDepth;
            }
            void setBias(float bias)
            {
                _bias = bias;
            }
            float getBias(void) const
            {
                return _bias;
            }
            void setBackGroundColor(Vec3f &backgroundColor)
            {
                _backgroundColor = backgroundColor;
            }
            Vec3f getBackGroundColor(void) const
            {
                return _backgroundColor;
            }

            bool intersect(
                const Vec3f &origin,
                const Vec3f &direction,
                primitive::intersectionInfo &isect) const override;

            void translate(Vec3f translation) override {};
            void rotateAroundX(float angle) override {};
            void rotateAroundY(float angle) override {};
            void rotateAroundZ(float angle) override {};
            void rotateAroundOriginX(float angle) override {};
            void rotateAroundOriginY(float angle) override {};
            void rotateAroundOriginZ(float angle) override {};
            void scale(float x, float y, float z) override {}
        private:
            bool trace(
                const Vec3f &origin, const Vec3f &direction,
                primitive::intersectionInfo &isect,
                physics::RayType rayType = physics::PRIMARY_RAY) const;
        private:
            uint8_t _maxDepth;
            float _bias;
            Vec3f _backgroundColor;

            const float _infinity = std::numeric_limits<float>::max();

            std::vector<std::shared_ptr<primitive::Object>> objects;
            std::vector<std::shared_ptr<physics::Light>> lights;
    };
}