/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** sphere.cpp
*/

#include "Plane.hpp"
#include "../../../shared/math/analysis/analysis.hpp"

namespace primitive {
    Plane::Plane(const Matrix44f &o2w, Vec3f vect1_o, Vec3f vect2_o, Vec3f vect1_d, Vec3f vect2_d) : a_o(vect1_o), a(vect1_d), b_o(vect2_o), b(vect2_d), Object(o2w) {}

    bool Plane::intersect(
        const Vec3f &origin,
        const Vec3f &direction,
        float &tnear,
        uint32_t &index,
        Vec2f &uv)
        const
    {
        //std::cout << "on rentre" << std::endl;
        // Vec3f ortho = {
        //     a.y * b.z - a.z * b.y,
        //     (-a.x) * b.z - a.z * b.x,
        //     a.x * b.y - a.y * b.x
        // };
        Vec3f normal = math::normalize(a);
        //Vec3f tmpB = math::normalize(b);
        //Vec3f normal = math::crossProduct(tmpA, tmpB);
        float denom = math::dotProduct(normal, direction);
        if (denom > 0) {
            Vec3f bOrig = math::normalize(b) - origin;
            tnear = math::dotProduct(bOrig, normal) / denom;
            //float t = math::dotProduct(originToPlane, normal) / denom;
            return (tnear >= 0);
        }
        return false;
        // int scalar_product = ortho.x * direction.x + ortho.y * direction.y + ortho.z * direction.z;
        // if (scalar_product == 0) {
        //     //std::cout << "sclara product 0" << std::endl;
        //     return false;
        // }

        // std::cout << scalar_product << std::endl;
        // float t = (originToPlane.x * ortho.x + originToPlane.y * ortho.y + originToPlane.z * ortho.z) / scalar_product;

        // if (t < 0) {
        //     //std::cout << "c'est le t wola" << std::endl;
        //     return false;
        // }
        // tnear = t;
        // std::cout << tnear << std::endl;
        // //std::cout << "eh ca ta mere" << std::endl;
        return true;
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
        //normal = -math::normalize(a);
    }
    
}