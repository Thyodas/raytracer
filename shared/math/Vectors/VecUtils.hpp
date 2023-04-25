/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** VecUtils.hpp
*/

#pragma once

#include "Vec3.hpp"
#include "Vec2.hpp"

#include <cmath>

namespace math {
    Vec3f normalize(const Vec3f &v);
    float dotProduct(const Vec3f &v1, const Vec3f &v2);
    Vec3f mix(const Vec3f &v1, const Vec3f &v2, const float &mixValue);
    Vec3f crossProduct(const Vec3f &v1, const Vec3f &v2);

    Vec2f normalize(const Vec2f &v);
    float dotProduct(const Vec2f &v1, const Vec2f &v2);
    Vec2f mix(const Vec2f &v1, const Vec2f &v2, const float &mixValue);
}