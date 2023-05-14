/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** sphere.cpp
*/

#include "Plane.hpp"
#include "../../../shared/math/analysis/analysis.hpp"

namespace primitive {
    Plane::Plane(const Matrix44f &o2w, Vec3f n, Vec3f p) :  normal(n), point(p), Object(o2w)
    {
        o2w.multVecMatrix(Vec3f(0), point);
        Matrix44f invert = o2w.inverse();
        Matrix44f transformNormal = invert.transposed();
        transformNormal.multDirMatrix(normal, normal);
        normal = math::normalize(normal);
    }

    bool Plane::intersect(
        const Vec3f &origin,
        const Vec3f &direction,
        float &tnear,
        uint32_t &index,
        Vec2f &uv)
        const
    {
        float denom = math::dotProduct(normal, direction);
        if (denom > 0) {
            Vec3f bOrig = point - origin;
            tnear = math::dotProduct(bOrig, normal) / denom;
            return (tnear >= 0);
        }
        return false;
    }

    void Plane::getSurfaceProperties(
            const Vec3f &point,
            const Vec3f &incident,
            const uint32_t &index,
            const Vec2f &uv,
            Vec3f &normal,
            Vec2f &textCoord)
    const
    {
        normal = -math::normalize(this->normal);

        Vec3f e1 = math::normalize(math::crossProduct(normal, Vec3f(1, 0, 0)));
        if (e1 == Vec3f(0))
            e1 = math::normalize(math::crossProduct(normal, Vec3f(0, 0, 1)));
        Vec3f e2 = math::normalize(math::crossProduct(normal, e1));
        if (point.x > 0)
            e2 = -e2;
        if (point.y > 0)
            e1 = -e1;
        textCoord.x = math::dotProduct(e1, point);
        textCoord.y = math::dotProduct(e2, point);
    }

}