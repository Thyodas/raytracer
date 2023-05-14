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
#include "../../../shared/math/Matrix/Matrix44.hpp"
#include "../../../shared/math/Matrix/MatrixTransformation.hpp"

namespace primitive {
    class Sphere : public Object {
        public:
            ~Sphere() = default;
            Sphere(const Matrix44f &o2w, const float &radius_);
            bool intersect(
                const Vec3f &origin,
                const Vec3f &direction,
                intersectionInfo &isect)
            const override;

            void translate(Vec3f translation) override;
            void rotateAroundX(float angle) override;
            void rotateAroundY(float angle) override;
            void rotateAroundZ(float angle) override;
            void rotateAroundOriginX(float angle) override;
            void rotateAroundOriginY(float angle) override;
            void rotateAroundOriginZ(float angle) override;
            void scale(float x, float y, float z) override;

        private:
            void getSphereProperties(
                const Vec3f &point,
                Vec3f &normal,
                Vec2f &textCoord)
            const;
            Vec3f evalSphereColor(const Vec2f &txtCoord) const;


        public:
            Vec3f center;
            float radius;
            float radius2;
    };
}