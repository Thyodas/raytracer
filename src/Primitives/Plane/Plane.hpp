/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** sphere.hpp
*/

#pragma once

#include "../../../include/Object.hpp"
#include "../../../shared/math/Vectors/Vec3.hpp"
#include "../../../shared/math/Vectors/Vec2.hpp"
#include "../../../shared/math/Vectors/VecUtils.hpp"

namespace primitive {
    class Plane : public Object {
        public:
            ~Plane() = default;
            Plane(const Matrix44f &o2w, Vec3f n, Vec3f p);
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

            void translate(Vec3f translation) override {};
            void rotateAroundX(float angle) override {};
            void rotateAroundY(float angle) override {};
            void rotateAroundZ(float angle) override {};
            void rotateAroundOriginX(float angle) override {};
            void rotateAroundOriginY(float angle) override {};
            void rotateAroundOriginZ(float angle) override {};
            void scale(float x, float y, float z) override {};

            Vec3f normal;
            Vec3f point;
    };
}