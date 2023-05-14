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
        primitive::intersectionInfo &isect,
        physics::RayType rayType) const
    {
        isect.hitObject = nullptr;
        for (uint32_t k = 0; k < objects.size(); ++k) {
            if (rayType == physics::SHADOW_RAY && objects[k]->materialType == primitive::REFLECTION_AND_REFRACTION)
                continue;
            float tmpTnear = std::numeric_limits<float>::max();
            if (objects[k]->intersect(origin, direction, isect) && isect.tNear < tmpTnear) {
                tmpTnear = isect.tNear;
            }
        }

        return (isect.hitObject != nullptr);
    }

    bool Scene::intersect(
                const Vec3f &origin,
                const Vec3f &direction,
                primitive::intersectionInfo &isect) const
    {
        return trace(origin, direction, isect);
    }

    Vec3f computeDiffuseColorChannel(Vec3f N, Vec3f lightDir, Vec3f lightIntensity)
    {
        float cosAngIncidence = math::dotProduct(N, -lightDir);
        return lightIntensity * std::max(0.f, cosAngIncidence);
    }

    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0, 1);

    Vec3f Scene::castRay(const Vec3f &orig, const Vec3f &dir, uint32_t depth)
    {
        if (depth > _maxDepth)
            return 0;
        primitive::intersectionInfo isect;
        Vec3f hitColor = _backgroundColor;
        Vec2f uv;
        uint32_t index = 0;
        if (trace(orig, dir, isect)) {
            Vec3f hitPoint = orig + dir * isect.tNear;
            Vec3f N = isect.normal;
            Vec3f color = isect.color;
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
                    hitColor = castRay(reflectionRayOrig, reflectionDirection, depth + 1) * kr;
                    break;
                }
                default:
                {
                Vec3f directLighting = 0;
                Vec3f specular = 0;
                for (uint32_t i = 0; i < lights.size(); ++i) {
                    Vec3f lightDir, lightIntensity;
                    primitive::intersectionInfo isectShad;
                    lights[i]->illuminate(hitPoint, lightDir, lightIntensity, isectShad.tNear);
                    bool vis = !trace(hitPoint + N * _bias, -lightDir, isectShad, physics::SHADOW_RAY);
                    //Compute diffuse component
                    directLighting += vis * lightIntensity * std::max(0.f, math::dotProduct(N, -lightDir));
                    Vec3f R = physics::reflect(lightDir, N);
                    //Compute specular component
                    directLighting += vis * lightIntensity * std::pow(std::max(0.f, math::dotProduct(R, -dir)), isect.hitObject->specularExponent);
                }
                Vec3f indirectLigthing = 0;
#if 1
                uint32_t nbSample = 8;
                Vec3f Nt, Nb;
                math::createCoordinateSystem(N, Nt, Nb);
                float pdf = 1 / (2 * M_PI);
                for (uint32_t n = 0; n < nbSample; ++n) {
                    float r1 = distribution(generator);
                    float r2 = distribution(generator);
                    Vec3f sample = math::uniformSampleHemisphere(r1, r2);
                    Vec3f sampleWorld(
                        sample.x * Nb.x + sample.y * N.x + sample.z * Nt.x,
                        sample.x * Nb.y + sample.y * N.y + sample.z * Nt.y,
                        sample.x * Nb.z + sample.y * N.z + sample.z * Nt.z);
                    indirectLigthing += r1 * castRay(hitPoint + sampleWorld * _bias,
                                                    sampleWorld, depth + 1) / pdf;
                }
                indirectLigthing /= (float)nbSample;
                directLighting /= M_PI;
#endif
                hitColor = (directLighting + indirectLigthing) * color;
                break;
                }
            }
        } else
            hitColor = _backgroundColor;
        return hitColor;
    }
}