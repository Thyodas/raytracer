/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** Object.hpp
*/

#pragma once

#include "../shared/math/Vectors/Vec3.hpp"
#include "../shared/math/Vectors/Vec2.hpp"
#include "../shared/math/Vectors/VecUtils.hpp"
#include "../shared/math/Matrix/Matrix44.hpp"
#include "Transformable.hpp"

#include <limits>

namespace primitive {
    enum MaterialType {
        DIFFUSE_AND_GLOSSY,
        REFLECTION_AND_REFRACTION,
        REFLECTION,
        NB_MATERIAL_TYPE,
    };

    enum Texture {
        DIFFUSE,
        TEXTURE,
        CHECKER,
        NB_TXT_TYPE,
    };

    struct intersectionInfo;
    class Object : public math::Transformable {
        public:
            virtual ~Object() = default;
            Object(const Matrix44f &o2w) :
                materialType(DIFFUSE_AND_GLOSSY),
                refractionCoefficient(1.3),
                kd(0.8),
                ks(0.2),
                specularExponent(25),
                objectToWorld(o2w),
                worldtoObject(o2w.inverse()) {};
            virtual bool intersect(
                const Vec3f &origin,
                const Vec3f &direction,
                intersectionInfo &isect) const = 0;

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
    struct intersectionInfo
    {
            const primitive::Object *hitObject = nullptr;
            float tNear = std::numeric_limits<float>::max();
            Vec2f uv;
            uint32_t index = 0;
            Vec3f normal;
            Vec3f color;
    };
}