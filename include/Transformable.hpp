/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** Transformable.hpp
*/

#pragma once

#include "../shared/math/Vectors/Vec3.hpp"

namespace math {
    class Transformable {
        public:
            virtual void translate(Vec3f translation) = 0;
            virtual void rotateAroundX(float angle) = 0;
            virtual void rotateAroundY(float angle) = 0;
            virtual void rotateAroundZ(float angle) = 0;
            virtual void rotateAroundOriginX(float angle) = 0;
            virtual void rotateAroundOriginY(float angle) = 0;
            virtual void rotateAroundOriginZ(float angle) = 0;
            virtual void scale(float x, float y, float z) = 0;
    };
}