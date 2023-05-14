/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** Cylindre.cpp
*/

#include "Cylindre.hpp"
#include "../../../shared/math/analysis/analysis.hpp"

namespace primitive
{
    Cylindre::Cylindre(const Matrix44f &o2w, float radius_, float height_) : Object(o2w), radius(radius_), height(height_)
    {
        o2w.multVecMatrix(Vec3f(0), center);
    }

    bool Cylindre::intersect(
        const Vec3f &origin,
        const Vec3f &direction,
        float &tnear,
        uint32_t &index,
        Vec2f &uv) const
    {
        float t0, t1;
        Vec3f L = origin - center;
        float a = direction.x * direction.x + direction.z * direction.z;
        float b = 2 * (direction.x * L.x + direction.z * L.z);
        float c = L.x * L.x + L.z * L.z - radius * radius;
        if (!math::solveQuadratic(a, b, c, t0, t1))
            return false;

        float y0 = origin.y + t0 * direction.y;
        float y1 = origin.y + t1 * direction.y;

        if (y0 >= 0 && y0 <= height)
        {
            tnear = t0;
            uv.x = std::atan2(L.x, L.z) / (2 * M_PI) + 0.5;
            uv.y = y0 / height;
            return true;
        }

        if (y1 >= 0 && y1 <= height)
        {
            tnear = t1;
            uv.x = std::atan2(L.x, L.z) / (2 * M_PI) + 0.5;
            uv.y = y1 / height;
            return true;
        }

        // Check for intersection with the top cap
        if (direction.y != 0)
        {
            float t = (0 - origin.y) / direction.y;
            float x = origin.x + t * direction.x;
            float z = origin.z + t * direction.z;
            if (x * x + z * z <= radius * radius && t > 0 && t < tnear)
            {
                tnear = t;
                uv.x = (x / radius + 1) * 0.5;
                uv.y = (z / radius + 1) * 0.5;
                return true;
            }
        }

        // Check for intersection with the bottom cap
        if (direction.y != 0)
        {
            float t = (height - origin.y) / direction.y;
            float x = origin.x + t * direction.x;
            float z = origin.z + t * direction.z;
            if (x * x + z * z <= radius * radius && t > 0 && t < tnear)
            {
                tnear = t;
                uv.x = (x / radius + 1) * 0.5;
                uv.y = (z / radius + 1) * 0.5;
                return true;
            }
        }
        return false;
    }

    void Cylindre::getSurfaceProperties(
        const Vec3f &point,
        const Vec3f &incident,
        const uint32_t &index,
        const Vec2f &uv,
        Vec3f &normal,
        Vec2f &textCoord) const
    {
        if (uv.y == 0)
        {
            normal = Vec3f(0, -1, 0); // Top cap normal
            textCoord = Vec2f(uv.x, uv.y);
        }
        else if (uv.y == 1)
        {
            normal = Vec3f(0, 1, 0); // Bottom cap normal
            textCoord = Vec2f(uv.x, uv.y);
        }
        else
        {
            float theta = 2 * M_PI * (uv.x - 0.5);
            normal = Vec3f(std::cos(theta), 0, std::sin(theta));
            textCoord = Vec2f(uv.x, uv.y);
        }
    }

    Vec3f Cylindre::evalDiffuseColor(const Vec2f &txtCoord) const
    {
        if (txtType == DIFFUSE)
            return albedo;
        if (txtType == CHECKER)
        {
            float scale = 5;
            float pattern = (fmodf(txtCoord.x * scale, 1) > 0.5) ^ (fmodf(txtCoord.y * scale, 1) > 0.5);
            return math::mix(Vec3f(0.0, 0.0, 0.0), Vec3f(1, 1, 1), pattern);
        }
        return Vec3f(0);
    }

    void Cylindre::translate(Vec3f translation)
    {
        math::translate(objectToWorld, translation);
        objectToWorld.multVecMatrix(center, center);
    }

    void Cylindre::rotateAroundX(float angle)
    {
        math::rotateX(objectToWorld, angle);
        objectToWorld.multVecMatrix(center, center);
    }

    void Cylindre::rotateAroundY(float angle)
    {
        math::rotateY(objectToWorld, angle);
        objectToWorld.multVecMatrix(center, center);
    }

    void Cylindre::rotateAroundZ(float angle)
    {
        math::rotateZ(objectToWorld, angle);
        objectToWorld.multVecMatrix(center, center);
    }

    void Cylindre::rotateAroundOriginX(float angle)
    {
        math::rotateAroundOriginX(objectToWorld, angle);
        objectToWorld.multVecMatrix(center, center);
    }

    void Cylindre::rotateAroundOriginY(float angle)
    {
        math::rotateAroundOriginY(objectToWorld, angle);
        objectToWorld.multVecMatrix(center, center);
    }

    void Cylindre::rotateAroundOriginZ(float angle)
    {
        math::rotateAroundOriginZ(objectToWorld, angle);
        objectToWorld.multVecMatrix(center, center);
    }

    void Cylindre::scale(float x, float y, float z)
    {
        math::scale(objectToWorld, x, y, z);
        objectToWorld.multVecMatrix(center, center);
    }
}