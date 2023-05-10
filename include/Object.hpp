/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** Object.hpp
*/

#pragma once

#include "../shared/math/Vectors/Vec3.hpp"
#include "../shared/math/Vectors/Vec2.hpp"
#include "../shared/math/Matrix/Matrix44.hpp"
#include "Transformable.hpp"

namespace primitive {
    enum MaterialType {
        DIFFUSE_AND_GLOSSY,
        REFLECTION_AND_REFRACTION,
        REFLECTION
    };

    enum Texture {
        DIFFUSE,
        TEXTURE,
        CHECKER
    };

    class Object : public math::Transformable {
        public:
            virtual ~Object() = default;
            Object(const Matrix44f &o2w) : materialType(DIFFUSE_AND_GLOSSY), refractionCoefficient(1.3), kd(0.8), ks(0.2), specularExponent(25), objectToWorld(o2w), worldtoObject(o2w.inverse()) {};
            virtual bool intersect(const Vec3f &, const Vec3f &, float &, uint32_t &, Vec2f &) const = 0;
            /**
             * Calculates the surface properties of a mesh triangle at a given point.
             *
             * @param point The point on the triangle to calculate the surface properties for.
             * @param incident The incident direction at the point.
             * @param index The index of the triangle in the mesh.
             * @param uv The texture coordinates at the point.
             * @param normal [out] The normal vector at the point.
             * @param textCoord [out] The texture coordinates at the point, interpolated from the triangle's vertices.
             */
            virtual void getSurfaceProperties(const Vec3f &point,
                                              const Vec3f &incident,
                                              const uint32_t &index,
                                              const Vec2f &uv,
                                              Vec3f &normal,
                                              Vec2f &textCoord) const = 0;
            virtual Vec3f evalDiffuseColor(const Vec2f &) const { return albedo; }

            MaterialType materialType = DIFFUSE_AND_GLOSSY;
            float refractionCoefficient = 1.5;
            float kd = 0.8;
            float ks = 0.2;
            float ka = 0.2;
            float specularExponent = 10;
            Vec3f albedo = 0.18;
            Matrix44f objectToWorld;
            Matrix44f worldtoObject;
            Texture txtType = DIFFUSE;
    };
}