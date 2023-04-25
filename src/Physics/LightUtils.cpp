/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** LightUtils.cpp
*/

#include "LightUtils.hpp"
#include "../../shared/math/utils.hpp"
#include <cmath>

namespace physics {
    Vec3f reflect(const Vec3f &incident, const Vec3f &normal)
    {
        return incident - 2 * math::dotProduct(incident, normal) * normal;
    }
    Vec3f refract(const Vec3f &incident, const Vec3f &normal, const float &refractionIndex)
    {
        float cosThetaI = math::clamp(-1, 1, math::dotProduct(incident, normal));
        float incomingRefractionIndex = 1;
        float outgoingRefractionIndex = refractionIndex;
        Vec3f n = normal;
        if (cosThetaI < 0)
            cosThetaI = -cosThetaI;
        else {
            std::swap(incomingRefractionIndex, outgoingRefractionIndex);
            n = -normal;
        }
        float ratio = incomingRefractionIndex / outgoingRefractionIndex;
        float k = 1 - ratio * ratio * (1 - cosThetaI * cosThetaI);
        return k < 0 ? 0 : ratio * incident + (ratio * cosThetaI - sqrtf(k)) * n;
    }
    void fresnel(const Vec3f &incident, const Vec3f &normal, const float &refractionIndex, float &kr)
    {
        float cosThetaI = math::clamp(-1, 1, math::dotProduct(incident, normal));
        float incomingRefractionIndex = 1;
        float outGoingRefractionIndex = refractionIndex;
        if (cosThetaI > 0)
            std::swap(incomingRefractionIndex, outGoingRefractionIndex);
        float sinThetaI = incomingRefractionIndex / outGoingRefractionIndex * sqrtf(std::max(0.f, 1 - cosThetaI * cosThetaI));
        if (sinThetaI >= 1)
            kr = 1;
        else {
            float cosThetaT = sqrtf(std::max(0.f, 1 - sinThetaI * sinThetaI));
            cosThetaI = fabsf(cosThetaI);
            float Rs = ((outGoingRefractionIndex * cosThetaI) - (incomingRefractionIndex * cosThetaT)) / ((outGoingRefractionIndex * cosThetaI) + (incomingRefractionIndex * cosThetaT));
            float Rp = ((incomingRefractionIndex * cosThetaI) - (outGoingRefractionIndex * cosThetaT)) / ((incomingRefractionIndex * cosThetaI) + (outGoingRefractionIndex * cosThetaT));
            kr = (Rs * Rs + Rp * Rp) / 2;
        }
    }
}