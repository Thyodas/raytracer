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

    bool Scene::trace(const Vec3f &orig, const Vec3f &dir,
                      float &tNear, uint32_t &index, Vec2f &uv, primitive::Object **hitObject)
    {
        *hitObject = nullptr;
        for (uint32_t k = 0; k < objects.size(); ++k) {
            float tNearK = _infinity;
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

    Vec3f Scene::castRay(const Vec3f &orig, const Vec3f &dir, uint32_t depth)
    {
        if (depth > _maxDepth)
            return _backgroundColor;
        Vec3f hitColor = _backgroundColor;
        float tnear = _infinity;
        Vec2f uv;
        uint32_t index = 0;
        primitive::Object *hitObject = nullptr;
        if (trace(orig, dir, tnear, index, uv, &hitObject)) {
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
                        hitPoint - N * _bias :
                        hitPoint + N * _bias;
                    Vec3f refractionRayOrig = (math::dotProduct(refractionDirection, N) < 0) ?
                        hitPoint - N * _bias :
                        hitPoint + N * _bias;
                    Vec3f reflectionColor = castRay(reflectionRayOrig, reflectionDirection, depth + 1);
                    Vec3f refractionColor = castRay(refractionRayOrig, refractionDirection, depth + 1);
                    float kr = 0;
                    physics::fresnel(dir, N, hitObject->refractionCoefficient, kr);
                    hitColor = reflectionColor * kr + refractionColor * (1 - kr);
                    break;
                }
                case primitive::REFLECTION:
                {
                    Vec3f reflectionDirection = math::normalize(physics::reflect(dir, N));
                    Vec3f reflectionRayOrig = (math::dotProduct(reflectionDirection, N) < 0) ?
                        hitPoint - N * _bias :
                        hitPoint + N * _bias;
                    Vec3f reflectionColor = castRay(reflectionRayOrig, reflectionDirection, depth + 1);
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
                        hitPoint + N * _bias :
                        hitPoint - N * _bias;
                    for (uint32_t i = 0; i < lights.size(); ++i) {
                        Vec3f lightDir = lights[i]->position - hitPoint;
                        float lightDistance2 = math::dotProduct(lightDir, lightDir);
                        lightDir = math::normalize(lightDir);
                        float LdotN = std::max(0.f, math::dotProduct(lightDir, N));
                        primitive::Object *shadowHitObject = nullptr;
                        float tNearShadow = _infinity;
                        bool inShadow = trace(shadowPointOrig, lightDir, tNearShadow, index, uv, &shadowHitObject) &&
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
}