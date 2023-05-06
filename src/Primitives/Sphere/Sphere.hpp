/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** sphere.hpp
*/

#pragma once

#include "Object.hpp"
#include "../../../shared/math/Vectors/Vec3.hpp"
#include "../../../shared/math/Vectors/Vec2.hpp"
#include "../../../shared/math/Vectors/VecUtils.hpp"

namespace primitive {
    class Sphere : public Object {
        public:
            ~Sphere() = default;
            Sphere(const Matrix44f &o2w, const float &radius_);
            bool intersect(
                const Vec3f &origin,
                const Vec3f &direction,
                float &tnear,
                uint32_t &index,
                Vec2f &uv)
            const override;
            void getSurfaceProperties(
                const Vec3f &point,
                const Vec3f &incident,
                const uint32_t &index,
                const Vec2f &uv,
                Vec3f &normal,
                Vec2f &textCoord)
            const override;

            Vec3f center;
            float radius;
            float radius2;
    };
}