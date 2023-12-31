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
                const Matrix44f &o2w,
                const std::vector<int> &faceIndex,
                const std::vector<uint32_t> &vertexIndex,
                const std::vector<Vec3f> &vertexArray,
                const std::vector<Vec3f> &normals = {},
                const std::vector<Vec2f> &st = {}
            );

            bool intersect(
                const Vec3f &origin,
                const Vec3f &direction,
                intersectionInfo &isect)
            const override;

            void translate(Vec3f translation) override;
            void rotateAroundX(float angle) override;
            void rotateAroundY(float angle) override;
            void rotateAroundZ(float angle) override;
            void rotateAroundOriginX(float angle) override;
            void rotateAroundOriginY(float angle) override;
            void rotateAroundOriginZ(float angle) override;
            void scale(float x, float y, float z) override;

            std::vector<Vec3f> _vertices;
            uint32_t _nbTriangles = 0;
            std::vector<uint32_t> _vertexIndex;
            std::vector<Vec2f> _textCoords;
            std::vector<Vec3f> _normalCoords;
        private:
            bool MollerTrumbore(
                const Vec3f &a, const Vec3f &b, const Vec3f &c,
                const Vec3f &origin, const Vec3f &direction,
                float &tnear, float &u, float &v
            ) const;

            void getTrianglesProperties(
                const uint32_t &index,
                const Vec2f &uv,
                Vec3f &normal,
                Vec2f &textCoord)
            const;

            Vec3f evalTriangleColor(const Vec2f &textCoord) const;
    };
}