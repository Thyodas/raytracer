/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** Scene.cpp
*/

#include "Scene.hpp"

namespace raytracer {

    void Scene::addObjectToScene(const std::shared_ptr<primitive::Object> &obj)
    {
        objects.push_back(obj);
    }

    void Scene::addLightToScene(const std::shared_ptr<physics::Light> &light)
    {
        lights.push_back(light);
    }

    bool Scene::trace(
        const Vec3f &origin, const Vec3f &direction,
        IsectInfo &isect,
        physics::RayType rayType)
    {
        isect.hitObject = nullptr;
        for (uint32_t k = 0; k < objects.size(); ++k) {
            float tNear = std::numeric_limits<float>::max();
            uint32_t index = 0;
            Vec2f uv;
            if (objects[k]->intersect(origin, direction, tNear, index, uv) && tNear < isect.tNear) {
                if (rayType == physics::SHADOW_RAY && objects[k]->materialType == primitive::REFLECTION_AND_REFRACTION)
                    continue;
                isect.hitObject = objects[k].get();
                isect.tNear = tNear;
                isect.index = index;
                isect.uv = uv;
            }
        }

        return (isect.hitObject != nullptr);
    }

    Vec3f computeDiffuseColorChannel(Vec3f N, Vec3f lightDir, Vec3f lightIntensity)
    {
        float cosAngIncidence = math::dotProduct(N, -lightDir);
        return lightIntensity * std::max(0.f, cosAngIncidence);
    }

    Vec3f Scene::castRay(const Vec3f &orig, const Vec3f &dir, uint32_t depth)
    {
        if (depth > _maxDepth)
            return _backgroundColor;
        IsectInfo isect;
        Vec3f hitColor = _backgroundColor;
        Vec2f uv;
        uint32_t index = 0;
        if (trace(orig, dir, isect)) {
            Vec3f hitPoint = orig + dir * isect.tNear;
            Vec3f N;
            Vec2f txtCoord;
            isect.hitObject->getSurfaceProperties(hitPoint, dir, index , uv, N, txtCoord);
            switch (isect.hitObject->materialType) {
                case primitive::REFLECTION_AND_REFRACTION:
                {
                    Vec3f reflectionDirection = math::normalize(physics::reflect(dir, N));
                    Vec3f refractionDirection = math::normalize(physics::refract(dir, N, isect.hitObject->refractionCoefficient));
                    Vec3f reflectionRayOrig = (math::dotProduct(reflectionDirection, N) < 0) ?
                        hitPoint - N * _bias :
                        hitPoint + N * _bias;
                    Vec3f refractionRayOrig = (math::dotProduct(refractionDirection, N) < 0) ?
                        hitPoint - N * _bias :
                        hitPoint + N * _bias;
                    Vec3f reflectionColor = castRay(reflectionRayOrig, reflectionDirection, depth + 1);
                    Vec3f refractionColor = castRay(refractionRayOrig, refractionDirection, depth + 1);
                    float kr = 0;
                    physics::fresnel(dir, N, isect.hitObject->refractionCoefficient, kr);
                    hitColor = reflectionColor * kr + refractionColor * (1 - kr);
                    break;
                }
                case primitive::REFLECTION:
                {
                    float kr = 0;
                    physics::fresnel(dir, N, isect.hitObject->refractionCoefficient, kr);
                    Vec3f reflectionDirection = physics::reflect(dir, N);
                    Vec3f reflectionRayOrig = (math::dotProduct(reflectionDirection, N) < 0) ?
                        hitPoint + N * _bias :
                        hitPoint - N * _bias;
                    hitColor = castRay(reflectionRayOrig, reflectionDirection, depth + 1);
                    break;
                }
                default:
                {
                    Vec3f diffuse(0);
                    Vec3f specular(0);
                    for (uint32_t i = 0; i < lights.size(); ++i) {
                        Vec3f lightDir(0);
                        Vec3f lightIntensity(0);
                        IsectInfo isectShad;
                        float tLight;
                        lights[i]->illuminate(hitPoint, lightDir, lightIntensity, tLight);
                        bool vis = !trace(hitPoint + N * _bias, -lightDir, isectShad, physics::SHADOW_RAY);
                        float cosAngIncidence = math::dotProduct(N, -lightDir);
                        diffuse += vis  * lightIntensity *
                            std::max(0.f, cosAngIncidence);
                        Vec3f R = physics::reflect(lightDir, N);
                        specular += vis * lightIntensity *
                            std::pow(std::max(0.f, math::dotProduct(R, -dir)),
                                    isect.hitObject->specularExponent);
                    }
                    hitColor = (diffuse *
                        isect.hitObject->evalDiffuseColor(txtCoord) + specular);
                    break;
                }
            }
        }
        return hitColor;
    }
}