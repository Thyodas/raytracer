/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** cone.cpp
*/

#include "Cone.hpp"
#include "../../../shared/math/analysis/analysis.hpp"

namespace primitive {
    Cone::Cone(const Matrix44f &o2w,
                  const float &radius_, const float &height_, const float &angle_, const Vec3f &axis_) : Object(o2w), radius(radius_), height(height_), angle(angle_), axis(axis_)
    {
        o2w.multVecMatrix(Vec3f(0), center);
    }

    bool Cone::intersect(
            const Vec3f &origin,
            const Vec3f &direction,
            float &tnear,
            __attribute__((unused))uint32_t &index,
            __attribute__((unused))Vec2f &uv)
    const
    {
        Vec3f L = origin - center;
// calcul par rapport à center, raduis et height
        // float a = direction.x * direction.x + direction.y * direction.y - direction.z * direction.z;
        // float b = 2 * origin.x * direction.x + 2 * origin.y * direction.y - 2 * origin.z * direction.z;
        // float c = origin.x * origin.x + origin.y * origin.y - origin.z * origin.z;
// calcul par rapport à center, radius, height, angle and axis
        float a = math::dotProduct(direction, axis) * math::dotProduct(direction, axis) - angle * angle;
        float b = 2 * (math::dotProduct(direction, axis) * math::dotProduct(L, axis) - math::dotProduct(direction, L) * angle * angle);
        float c = math::dotProduct(L, axis) * math::dotProduct(L, axis) - math::dotProduct(L, L) * angle * angle;

        float t0 = 0;
        float t1 = 0;
        if (!math::solveQuadratic(a, b, c, t0, t1)) {
            std::cout << "false" << std::endl;
            return false;
        }
        if (t0 > t1)
            std::swap(t0, t1);
        if (t0 < 0) {
            t0 = t1;
            if (t0 < 0) {
                std::cout << "false" << std::endl;
                return false;
            }
        }
        tnear = t0;
        return true;
    }
    void Cone::getSurfaceProperties(
            const Vec3f &point,
            __attribute__((unused))const Vec3f &incident,
            __attribute__((unused))const uint32_t &index,
            __attribute__((unused))const Vec2f &uv,
            Vec3f &normal,
            Vec2f &textCoord)
    const
    {
        normal = math::normalize(point - center);

        textCoord.x = (1 + atan2(normal.z, normal.x) / M_PI) * 0.5;
        textCoord.y = acosf(normal.y) / M_PI;
    }

    Vec3f Cone::evalDiffuseColor(const Vec2f &txtCoord) const
    {
        if (txtType == DIFFUSE)
            return albedo;
        if (txtType == CHECKER) {
            float scale = 5;
            float pattern = (fmodf(txtCoord.x * scale, 1) > 0.5) ^ (fmodf(txtCoord.y * scale, 1) > 0.5);
            return math::mix(Vec3f(0.0, 0.0, 0.0), Vec3f(1, 1, 1), pattern);
        }
        return Vec3f(0);
    }

    void Cone::translate(Vec3f translation)
    {
        math::translate(objectToWorld, translation);
        objectToWorld.multVecMatrix(center, center);
    }

    void Cone::rotateAroundX(float angle)
    {
        math::rotateX(objectToWorld, angle);
        objectToWorld.multVecMatrix(center, center);
    }

    void Cone::rotateAroundY(float angle)
    {
        math::rotateY(objectToWorld, angle);
        objectToWorld.multVecMatrix(center, center);
    }

    void Cone::rotateAroundZ(float angle)
    {
        math::rotateZ(objectToWorld, angle);
        objectToWorld.multVecMatrix(center, center);
    }

    void Cone::rotateAroundOriginX(float angle)
    {
        math::rotateAroundOriginX(objectToWorld, angle);
        objectToWorld.multVecMatrix(center, center);
    }

    void Cone::rotateAroundOriginY(float angle)
    {
        math::rotateAroundOriginY(objectToWorld, angle);
        objectToWorld.multVecMatrix(center, center);
    }

    void Cone::rotateAroundOriginZ(float angle)
    {
        math::rotateAroundOriginZ(objectToWorld, angle);
        objectToWorld.multVecMatrix(center, center);
    }

    void Cone::scale(float x, float y, float z)
    {
        math::scale(objectToWorld, x, y , z);
        objectToWorld.multVecMatrix(center, center);
    }
}