/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** Light.hpp
*/

#pragma once

#include "../shared/math/Vectors/Vec3.hpp"
#include "../shared/math/Vectors/VecUtils.hpp"
#include "../shared/math/Matrix/Matrix44.hpp"
#include "../shared/math/utils.hpp"
#include <limits>
#include <cmath>

namespace physics {

    enum RayType {
        PRIMARY_RAY,
        SHADOW_RAY
    };
    class Light {
        public:
            ~Light() = default;
            Light(const Matrix44f &l2w, const Vec3f &c, const float i) : lightToWorld(l2w), color(c), intensity(i) {};
            virtual void illuminate(const Vec3f &point, Vec3f &lightDirection, Vec3f &lightIntensity, float &distance) const = 0;
            Matrix44f lightToWorld;
            Vec3f color;
            float intensity;
    };

    class DistantLight : public Light {
        public:
            DistantLight(const Matrix44f &l2w, const Vec3f &color, const float intensity) : Light(l2w, color, intensity)
            {
                l2w.multDirMatrix(Vec3f(0, 0, -1), dir);
                dir = math::normalize(dir);
            }
            void illuminate(__attribute__((unused))const Vec3f &point, Vec3f &lightDirection, Vec3f &lightIntensity, float &distance) const override
            {
                lightDirection = dir;
                lightIntensity = color * intensity;
                distance = std::numeric_limits<float>::max();
            }
            Vec3f dir;
    };

    class PointLight : public Light {
        public:
            PointLight(const Matrix44f &l2w, const Vec3f &color, const float intensity) : Light(l2w, color, intensity)
            {
                l2w.multVecMatrix(Vec3f(0, 0, -1), pos);
            }
            void illuminate(const Vec3f &point, Vec3f &lightDirection, Vec3f &lightIntensity, float &distance) const override
            {
                lightDirection = point - pos;
                float r2 = math::dotProduct(lightDirection, lightDirection);
                distance = sqrt(r2);
                lightDirection.x /= distance;
                lightDirection.y /= distance;
                lightDirection.z /= distance;
                Vec3f tmp = color * intensity;
                tmp.x /= 4 * M_PI * r2;
                tmp.y /= 4 * M_PI * r2;
                tmp.z /= 4 * M_PI * r2;
                lightIntensity = tmp;
            }
            Vec3f pos;
    };
}