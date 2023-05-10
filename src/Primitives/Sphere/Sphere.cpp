/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** sphere.cpp
*/

#include "Sphere.hpp"
#include "../../../shared/math/analysis/analysis.hpp"

namespace primitive {
    Sphere::Sphere(const Matrix44f &o2w,
                  const float &radius_) : Object(o2w), radius(radius_), radius2(radius_ * radius_)
    {
        o2w.multVecMatrix(Vec3f(0), center);
    }

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
        if (t0 > t1)
            std::swap(t0, t1);
        if (t0 < 0) {
            t0 = t1;
            if (t0 < 0)
                return false;
        }
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

        textCoord.x = (1 + atan2(normal.z, normal.x) / M_PI) * 0.5;
        textCoord.y = acosf(normal.y) / M_PI;
    }

    Vec3f Sphere::evalDiffuseColor(const Vec2f &txtCoord) const
    {
        if (txtType == DIFFUSE)
            return albedo;
        if (txtType == CHECKER) {
            float scale = 5;
            float pattern = (fmodf(txtCoord.x * scale, 1) > 0.5) ^ (fmodf(txtCoord.y * scale, 1) > 0.5);
            return math::mix(Vec3f(0.0, 0.0, 0.0), Vec3f(1, 1, 1), pattern);
        }
    }
}