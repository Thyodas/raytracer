/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** Core.cpp
*/

#include "Core.hpp"
#include <cmath>
#include <iostream>
#include <fstream>
#include <limits>

namespace raytracer {
    const float kInfinity = std::numeric_limits<float>::max();

    bool trace(
        const Vec3f &orig, const Vec3f &dir,
        const std::vector<std::unique_ptr<primitive::Object>> &objects,
        float &tNear, uint32_t &index, Vec2f &uv, primitive::Object **hitObject)
    {
        *hitObject = nullptr;
        for (uint32_t k = 0; k < objects.size(); ++k) {
            float tNearK = kInfinity;
            uint32_t indexK;
            Vec2f uvK;
            if (objects[k]->intersect(orig, dir, tNearK, indexK, uvK) && tNearK < tNear) {
                *hitObject = objects[k].get();
                tNear = tNearK;
                index = indexK;
                uv = uvK;
            }
        }

        return (*hitObject != nullptr);
    }

    Vec3f castRay(
        const Vec3f &orig, const Vec3f &dir,
        const std::vector<std::unique_ptr<primitive::Object>> &objects,
        const std::vector<std::unique_ptr<physics::Light>> &lights,
        const Options &options,
        uint32_t depth)
    {
        if (depth > options.maxDepth)
            return options.backgroundColor;
        Vec3f hitColor = options.backgroundColor;
        float tnear = kInfinity;
        Vec2f uv;
        uint32_t index = 0;
        primitive::Object *hitObject = nullptr;
        if (trace(orig, dir, objects, tnear, index, uv, &hitObject)) {
            Vec3f hitPoint = orig + dir * tnear;
            Vec3f N;
            Vec2f txtCoord;
            hitObject->getSurfaceProperties(hitPoint, dir, index, uv, N, txtCoord);
            switch (hitObject->materialType) {
                case primitive::REFLECTION_AND_REFRACTION:
                {
                    Vec3f reflectionDirection = math::normalize(physics::reflect(dir, N));
                    Vec3f refractionDirection = math::normalize(physics::refract(dir, N, hitObject->refractionCoefficient));
                    Vec3f reflectionRayOrig = (math::dotProduct(reflectionDirection, N) < 0) ?
                        hitPoint - N * options.bias :
                        hitPoint + N * options.bias;
                    Vec3f refractionRayOrig = (math::dotProduct(refractionDirection, N) < 0) ?
                        hitPoint - N * options.bias :
                        hitPoint + N * options.bias;
                    Vec3f reflectionColor = castRay(reflectionRayOrig, reflectionDirection, objects, lights, options, depth + 1);
                    Vec3f refractionColor = castRay(refractionRayOrig, refractionDirection, objects, lights, options, depth + 1);
                    float kr = 0;
                    physics::fresnel(dir, N, hitObject->refractionCoefficient, kr);
                    hitColor = reflectionColor * kr + refractionColor * (1 - kr);
                    break;
                }
                case primitive::REFLECTION:
                {
                    Vec3f reflectionDirection = math::normalize(physics::reflect(dir, N));
                    Vec3f reflectionRayOrig = (math::dotProduct(reflectionDirection, N) < 0) ?
                        hitPoint - N * options.bias :
                        hitPoint + N * options.bias;
                    Vec3f reflectionColor = castRay(reflectionRayOrig, reflectionDirection, objects, lights, options, depth + 1);
                    float kr = 0;
                    physics::fresnel(dir, N, hitObject->refractionCoefficient, kr);
                    hitColor = reflectionColor * kr;
                    break;
                }
                default:
                {
                    Vec3f lightAmt = 0;
                    Vec3f specularColor = 0;
                    Vec3f shadowPointOrig = (math::dotProduct(dir, N) < 0) ?
                        hitPoint + N * options.bias :
                        hitPoint - N * options.bias;
                    for (uint32_t i = 0; i < lights.size(); ++i) {
                        Vec3f lightDir = lights[i]->position - hitPoint;
                        float lightDistance2 = math::dotProduct(lightDir, lightDir);
                        lightDir = math::normalize(lightDir);
                        float LdotN = std::max(0.f, math::dotProduct(lightDir, N));
                        primitive::Object *shadowHitObject = nullptr;
                        float tNearShadow = kInfinity;
                        bool inShadow = trace(shadowPointOrig, lightDir, objects, tNearShadow, index, uv, &shadowHitObject) &&
                            tNearShadow * tNearShadow < lightDistance2;
                        lightAmt += (1 - inShadow) * lights[i]->intensity * LdotN;
                        Vec3f reflectionDirection = physics::reflect(-lightDir, N);
                        specularColor += powf(std::max(0.f, -math::dotProduct(reflectionDirection, dir)), hitObject->specularExponent) * lights[i]->intensity;
                    }
                    hitColor = lightAmt * hitObject->evalDiffuseColor(txtCoord) * hitObject->kd + specularColor * hitObject->ks;
                    break;
                }
            }
        }

        return hitColor;
    }
    void render(
        const Options &options,
        const std::vector<std::unique_ptr<primitive::Object>> &objects,
        const std::vector<std::unique_ptr<physics::Light>> &lights)
    {
        Vec3f *framebuffer = new Vec3f[options.width * options.height];
        float scale = tan(math::deg2Rad(options.fov * 0.5));
        float imageAspectRatio = options.width / (float)options.height;
        Vec3f orig(0);
        uint32_t index = 0;
        for (uint32_t j = 0; j < options.height; ++j) {
            for (uint32_t i = 0; i < options.width; ++i) {
                float x = (2 * (i + 0.5) / (float)options.width - 1) * imageAspectRatio * scale;
                float y = (1 - 2 * (j + 0.5) / (float)options.height) * scale;
                Vec3f dir = math::normalize(Vec3f(x, y, -1));
                framebuffer[index++] = castRay(orig, dir, objects, lights, options, 0);
            }
        }

        std::ofstream ofs;
        ofs.open("./out.ppm");
        ofs << "P6\n" << options.width << " " << options.height << "\n255\n";
        for (uint32_t i = 0; i < options.height * options.width; ++i) {
            char r = (char)(255 * math::clamp(0, 1, framebuffer[i].x));
            char g = (char)(255 * math::clamp(0, 1, framebuffer[i].y));
            char b = (char)(255 * math::clamp(0, 1, framebuffer[i].z));
            ofs << r << g << b;
        }

        ofs.close();

        delete [] framebuffer;
    }
}