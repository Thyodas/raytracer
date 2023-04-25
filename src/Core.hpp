/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** Core.hpp
*/

#include "Object.hpp"
#include "Light.hpp"
#include "Primitives/Sphere/Sphere.hpp"
#include "Primitives/MeshTriangle/MeshTriangle.hpp"
#include "../shared/math/utils.hpp"
#include "Physics/LightUtils.hpp"

#include <memory>

namespace raytracer {
    struct Options
    {
        uint32_t width;
        uint32_t height;
        float fov;
        float imageAspectRatio;
        uint8_t maxDepth;
        Vec3f backgroundColor;
        float bias;
    };
    void render(
        const Options &options,
        const std::vector<std::unique_ptr<primitive::Object>> &objects,
        const std::vector<std::unique_ptr<physics::Light>> &lights);
    Vec3f castRay(
        const Vec3f &orig, const Vec3f &dir,
        const std::vector<std::unique_ptr<primitive::Object>> &objects,
        const std::vector<std::unique_ptr<physics::Light>> &lights,
        const Options &options,
        uint32_t depth);
    bool trace(
        const Vec3f &orig, const Vec3f &dir,
        const std::vector<std::unique_ptr<primitive::Object>> &objects,
        float &tNear, uint32_t &index, Vec2f &uv, primitive::Object **hitObject);
}