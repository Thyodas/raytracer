/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** MeshTriangle.cpp
*/

#include "MeshTriangle.hpp"
#include <cstring>

namespace primitive {
    MeshTriangle::MeshTriangle(
        std::vector<Vec3f> &verts,
        std::vector<uint32_t> &vertsIndex,
        const uint32_t &numTris,
        std::vector<Vec2f> &st)
    {
        vertices = verts;
        vertexIndex = vertsIndex;
        textCoords = st;
        numTriangles = numTris;
    }

    bool MeshTriangle::MollerTrumbore(
                const Vec3f &a, const Vec3f &b, const Vec3f &c,
                const Vec3f &origin, const Vec3f &direction,
                float &tnear, float &u, float &v)
    const
    {
        Vec3f v0 = b - a;
        Vec3f v1 = c - a;
        Vec3f pvec = math::crossProduct(direction, v1);
        float det = math::dotProduct(v0, pvec);
        if (det == 0 || det < 0)
            return false;
        Vec3f tvec = origin - a;
        u = math::dotProduct(tvec, pvec);
        if (u < 0 || u > det)
            return false;
        Vec3f qvec = math::crossProduct(tvec, v0);
        v = math::dotProduct(direction, qvec);
        if (v < 0 || u + v > det)
            return false;
        float invDet = 1 / det;
        tnear = math::dotProduct(v1, qvec) * invDet;
        u *= invDet;
        v *= invDet;
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
        bool intersect = false;
        for (uint32_t i = 0; i < numTriangles; ++i) {
            const Vec3f &a = vertices[vertexIndex[i * 3]];
            const Vec3f &b = vertices[vertexIndex[i * 3 + 1]];
            const Vec3f &c = vertices[vertexIndex[i * 3 + 2]];
            float t = 0;
            float u = 0;
            float v = 0;
            if (MollerTrumbore(a, b, c, origin, direction, t, u, v) && t < tnear) {
                tnear = t;
                uv.x = u;
                uv.y = v;
                index = i;
                intersect |= true;
            }
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
        const Vec3f &a = vertices[vertexIndex[index * 3]];
        const Vec3f &b = vertices[vertexIndex[index * 3 + 1]];
        const Vec3f &c = vertices[vertexIndex[index * 3 + 2]];
        Vec3f v0 = math::normalize(b - a);
        Vec3f v1 = math::normalize(c - a);
        normal = math::normalize(math::crossProduct(v0, v1));

        //Texture coordinates
        const Vec2f &txt0 = textCoords[vertexIndex[index * 3]];
        const Vec2f &txt1 = textCoords[vertexIndex[index * 3 + 1]];
        const Vec2f &txt2 = textCoords[vertexIndex[index * 3 + 2]];
        textCoord = txt0 * (1 - uv.x - uv.y) + txt1 * uv.x + txt2 * uv.y;
    }

    Vec3f MeshTriangle::evalDiffuseColor(const Vec2f &textCoord) const
    {
        float scale = 5;
        float pattern = (fmodf(textCoord.x * scale, 1) > 0.5) ^ (fmodf(textCoord.y * scale, 1) > 0.5);
        return math::mix(Vec3f(0.0, 0.0, 0.0), Vec3f(1, 1, 1), pattern);
    }
}