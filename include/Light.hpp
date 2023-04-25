/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** Light.hpp
*/

#pragma once

#include "../shared/math/Vectors/Vec3.hpp"

namespace physics {
    class Light {
        public:
            ~Light() = default;
            Light(const Vec3f &pos, const Vec3f &intens) : position(pos), intensity(intens) {};

            Vec3f position;
            Vec3f intensity;
    };
}