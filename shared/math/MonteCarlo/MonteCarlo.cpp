/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** MonteCarlo.cpp
*/

#include "MonteCarlo.hpp"
#include "../Vectors/VecUtils.hpp"

namespace math {

    void createCoordinateSystem(const Vec3f &N, Vec3f &Nt, Vec3f &Nb)
    {
        if (std::fabs(N.x) > std::fabs(N.y))
            Nt = Vec3f(N.z, 0, -N.x) / sqrtf(N.x * N.x + N.z * N.z);
        else
            Nt = Vec3f(0, -N.z, N.y) / sqrtf(N.y * N.y + N.z * N.z);
        Nb = crossProduct(N, Nt);
    }

    Vec3f uniformSampleHemisphere(const float &r1, const float &r2)
    {
        float sinTheta = sqrtf(1 - r1 * r1);
        float phi = 2 * M_PI * r2;
        float x = sinTheta * cosf(phi);
        float z = sinTheta * sinf(phi);
        return Vec3f(x, r1, z);
    }
}