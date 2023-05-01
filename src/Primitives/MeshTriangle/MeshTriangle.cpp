/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** MeshTriangle.cpp
*/

#include "MeshTriangle.hpp"
#include <cstring>
#include <algorithm>
#include <limits>

namespace primitive {

    MeshTriangle::MeshTriangle(
        const std::vector<int> &faceIndex,
        const std::vector<uint32_t> &vertexIndex,
        const std::vector<Vec3f> &vertexArray,
        const std::vector<Vec3f> &normals,
        const std::vector<Vec2f> &st
    ) : Object()
    {
        uint32_t k = 0;
        uint32_t maxVertexIndex = 0;
        for (size_t i = 0; i < faceIndex.size(); ++i) {
            _nbTriangles += faceIndex[i] - 2;
            for (uint32_t j = 0; j < faceIndex[i]; ++j)
                if (vertexIndex[k + j] > maxVertexIndex)
                    maxVertexIndex = vertexIndex[k + j];
            k += faceIndex[i];
        }
        maxVertexIndex++;
        for (uint32_t i = 0; i < maxVertexIndex; ++i) {
            _vertices.push_back(vertexArray[i]);
        }
        uint32_t l = 0;
        k = 0;
        for (size_t i = 0; i < faceIndex.size(); ++i) {
            for (uint32_t j = 0; j < faceIndex[i] - 2; ++j) {
                _vertexIndex.push_back(vertexIndex[k]);
                _vertexIndex.push_back(vertexIndex[k + j + 1]);
                _vertexIndex.push_back(vertexIndex[k + j + 2]);
                if (!normals.empty()) {
                    _normalCoords.push_back(normals[k]);
                    _normalCoords.push_back(normals[k + j + 1]);
                    _normalCoords.push_back(normals[k + j + 2]);
                    _normalCoords[l] = math::normalize(_normalCoords[l]);
                    _normalCoords[l + 1] = math::normalize(_normalCoords[l + 1]);
                    _normalCoords[l + 2] = math::normalize(_normalCoords[l + 2]);
                }
                if (!st.empty()) {
                    _textCoords.push_back(st[k]);
                    _textCoords.push_back(st[k + j + 1]);
                    _textCoords.push_back(st[k + j + 2]);
                }
                l += 3;
            }
            k += faceIndex[i];
        }
        if (!_textCoords.empty())
            return;
        for (size_t i = 0; i < _vertices.size(); ++i) {
            if (i % 4 == 0)
                _textCoords.push_back(Vec2f(0, 0));
            if (i % 4 == 1)
                _textCoords.push_back(Vec2f(1, 0));
            if (i % 4 == 2)
                _textCoords.push_back(Vec2f(1, 1));
            if (i % 4 == 3)
                _textCoords.push_back(Vec2f(0, 1));
        }
    }

    bool MeshTriangle::MollerTrumbore(
                const Vec3f &a, const Vec3f &b, const Vec3f &c,
                const Vec3f &origin, const Vec3f &direction,
                float &tnear, float &u, float &v)
    const
    {
        const float espilon = 1e-8;
        Vec3f v0 = b - a;
        Vec3f v1 = c - a;
        Vec3f pvec = math::crossProduct(direction, v1);
        float det = math::dotProduct(v0, pvec);
        if (det < espilon)
            return false;
        float invDet = 1 / det;
        Vec3f tvec = origin - a;
        u = math::dotProduct(tvec, pvec) * invDet;
        if (u < 0 || u > 1)
            return false;
        Vec3f qvec = math::crossProduct(tvec, v0);
        v = math::dotProduct(direction, qvec) * invDet;
        if (v < 0 || u + v > 1)
            return false;
        tnear = math::dotProduct(v1, qvec) * invDet;
        return true;
    }

    bool MeshTriangle::intersect(
            const Vec3f &origin,
            const Vec3f &direction,
            float &tnear,
            uint32_t &index,
            Vec2f &uv)
    const
    {
        uint32_t j = 0;
        bool intersect = false;
        for (uint32_t i = 0; i < _nbTriangles; ++i) {
            const Vec3f &a = _vertices[_vertexIndex[j]];
            const Vec3f &b = _vertices[_vertexIndex[j + 1]];
            const Vec3f &c = _vertices[_vertexIndex[j + 2]];
            float t = std::numeric_limits<float>::max();
            float u = 0;
            float v = 0;
            if (MollerTrumbore(a, b, c, origin, direction, t, u, v) && t < tnear) {
                tnear = t;
                uv.x = u;
                uv.y = v;
                index = i;
                intersect |= true;
            }
            j += 3;
        }

        return intersect;
    }
    void MeshTriangle::getSurfaceProperties(
                const Vec3f &point,
                const Vec3f &incident,
                const uint32_t &index,
                const Vec2f &uv,
                Vec3f &normal,
                Vec2f &textCoord)
    const
    {
        //Face normal
        const Vec3f &a = _vertices[_vertexIndex[index * 3]];
        const Vec3f &b = _vertices[_vertexIndex[index * 3 + 1]];
        const Vec3f &c = _vertices[_vertexIndex[index * 3 + 2]];
        Vec3f v0 = b - a;
        Vec3f v1 = c - a;
        normal = math::normalize(math::crossProduct(v1, v0));

        //Texture coordinates
        if (!_textCoords.empty()) {
            const Vec2f &txt0 = _textCoords[_vertexIndex[index * 3]];
            const Vec2f &txt1 = _textCoords[_vertexIndex[index * 3 + 1]];
            const Vec2f &txt2 = _textCoords[_vertexIndex[index * 3 + 2]];
            textCoord = txt0 * (1 - uv.x - uv.y) + txt1 * uv.x + txt2 * uv.y;
        }
    }

    Vec3f MeshTriangle::evalDiffuseColor(const Vec2f &textCoord) const
    {
        float scale = 5;
        float pattern = (fmodf(textCoord.x * scale, 1) > 0.5) ^ (fmodf(textCoord.y * scale, 1) > 0.5);
        return math::mix(Vec3f(0.0, 0.0, 0.0), Vec3f(1, 1, 1), pattern);
    }
}