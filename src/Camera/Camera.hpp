/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** Camera.hpp
*/

#pragma once

#include "Transformable.hpp"
#include "../../shared/math/Vectors/Vec3.hpp"
#include "../../shared/math/Vectors/VecUtils.hpp"
#include "../../shared/math/Matrix/Matrix44.hpp"
#include "../../shared/math/Matrix/MatrixTransformation.hpp"
#include "../../shared/math/utils.hpp"

namespace raytracer {
    class Camera : public math::Transformable {
        public:
            Camera(Matrix44f &c2w, float width, float height, float fov);
            ~Camera() = default;

            void getDir(float i, float j, Vec3f &dir) const;

            void translate(Vec3f translation) override;
            void rotateAroundX(float angle) override;
            void rotateAroundY(float angle) override;
            void rotateAroundZ(float angle) override;
            void rotateAroundOriginX(float angle) override;
            void rotateAroundOriginY(float angle) override;
            void rotateAroundOriginZ(float angle) override;
            void scale(float x, float y, float z) override;
        public:
            Vec3f orig;
            Matrix44f cameraToWorld;
            float imageAspectRatio;
            float scaleRatio;
            float width;
            float height;
            float fov;
    };
}