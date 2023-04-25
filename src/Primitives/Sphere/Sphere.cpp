/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** sphere.cpp
*/

#include "Sphere.hpp"
#include "../../../shared/math/analysis/analysis.hpp"

namespace primitive {
    Sphere::Sphere(const Vec3f &center_, const float &radius_) : center(center_), radius(radius_), radius2(radius_ * radius_) {}

    bool Sphere::intersect(
            const Vec3f &origin,
            const Vec3f &direction,
            float &tnear,
            uint32_t &index,
            Vec2f &uv)
    const
    {
        Vec3f L = origin - center;
        float a = math::dotProduct(direction, direction);
        float b = 2 * math::dotProduct(direction, L);
        float c = math::dotProduct(L, L) - radius2;
        float t0 = 0;
        float t1 = 0;
        if (!math::solveQuadratic(a, b, c, t0, t1))
            return false;
        if (t0 < 0)
            t0 = t1;
        if (t0 < 0)
            return false;
        tnear = t0;
        return true;
    }
    void Sphere::getSurfaceProperties(
            const Vec3f &point,
            const Vec3f &incident,
            const uint32_t &index,
            const Vec2f &uv,
            Vec3f &normal,
            Vec2f &textCoord)
    const
    {
        normal = math::normalize(point - center);
    }
}