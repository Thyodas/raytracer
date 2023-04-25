/*
** EPITECH PROJECT, 2022
** B-OOP-400-STG-4-1-raytracer-marie.giacomel
** File description:
** LightUtils.hpp
*/

#include "../../shared/math/Vectors/Vec3.hpp"
#include "../../shared/math/Vectors/VecUtils.hpp"

namespace physics {
    Vec3f reflect(const Vec3f &incident, const Vec3f &normal);
    Vec3f refract(const Vec3f &incident, const Vec3f &normal, const float &refractionIndex);
    void fresnel(const Vec3f &incident, const Vec3f &normal, const float &refractionIndex, float &kr);
}