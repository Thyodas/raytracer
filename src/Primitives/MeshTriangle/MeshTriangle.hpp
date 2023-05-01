/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** MeshTriangle.hpp
*/

#pragma once

#include "Object.hpp"
#include "../../../shared/math/Vectors/Vec3.hpp"
#include "../../../shared/math/Vectors/Vec2.hpp"
#include "../../../shared/math/Vectors/VecUtils.hpp"

#include <vector>
#include <memory>

namespace primitive {
    class MeshTriangle : public Object {
        public:
            ~MeshTriangle() = default;
            MeshTriangle(
                std::vector<Vec3f> &verts,
                std::vector<uint32_t> &vertsIndex,
                const uint32_t &numTris,
                std::vector<Vec2f> &st);

            bool intersect(
                const Vec3f &origin,
                const Vec3f &direction,
                float &tnear,
                uint32_t &index,
                Vec2f &uv)
            const override;

            void getSurfaceProperties(
                const Vec3f &point,
                const Vec3f &incident,
                const uint32_t &index,
                const Vec2f &uv,
                Vec3f &normal,
                Vec2f &textCoord)
            const override;

            Vec3f evalDiffuseColor(const Vec2f &textCoord) const override;

            std::vector<Vec3f> vertices;
            uint32_t numTriangles;
            std::vector<uint32_t> vertexIndex;
            std::vector<Vec2f> textCoords;
        private:
            bool MollerTrumbore(
                const Vec3f &a, const Vec3f &b, const Vec3f &c,
                const Vec3f &origin, const Vec3f &direction,
                float &tnear, float &u, float &v
            ) const;
    };
}