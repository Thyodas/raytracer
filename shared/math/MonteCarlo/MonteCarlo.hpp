/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** MonteCarlo.hpp
*/

#pragma once

#include "../Vectors/Vec3.hpp"
#include <random>

namespace math {
    Vec3f uniformSampleHemisphere(const float &r1, const float &r2);
    void createCoordinateSystem(const Vec3f &N, Vec3f &Nt, Vec3f &Nb);
}